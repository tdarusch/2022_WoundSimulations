import xlsxwriter
import os
import math

workbook = xlsxwriter.Workbook('NewDynamicShapeParamDat.xlsx')
worksheet = workbook.add_worksheet()

root = "C:/Users/Tom/Desktop/Research"
os.chdir(root)
folder = os.path.join(root, r'\NewDynamicShapeParamTests')

simulationContainer = []

#simulation has an array w each individual timestep, an array w each average at each timestep, overall average, cells on border, and gamma
class Simulation:
    def __init__(self):
        self.AvgTimeStepData = []
        self.BorderCellsTimeStepData = []
        self.gamma = 0.0
        self.totalAvg = 0.0

    def addParamData(self, dati):
        self.AvgTimeStepData.append(dati)
    def addBorderData(self, dat):
        self.BorderCellsTimeStepData.append(dat)
    def setGamma(self, g):
        self.gamma = g
    def calcAvg(self):
        iter = 0.0
        sigma = 0.0
        for data in self.AvgTimeStepData:
            sigma += data.getData()
            iter += 1
        self.totalAvg = sigma/(float(len(self.AvgTimeStepData)))
    
    def getAvg(self):
        return self.totalAvg
    def getParamDataAtTime(self, t):
        return self.AvgTimeStepData[t]
    def getBorderCellsAtTime(self, t):
        return self.BorderCellsTimeStepData[t]
    def getGamma(self):
        return self.gamma
    def getBorderArray(self):
        return self.BorderCellsTimeStepData
    def getParamArray(self):
        return self.AvgTimeStepData
    def getLen(self):
        return len(self.AvgTimeStepData)

class Data:
    def __init__(self, datum = 0.0, cT = 0.0):
        self.cTime = cT
        self.data = datum
    
    def getData(self):
        return self.data
    def getTime(self):
        return self.cTime

    def setTime(self, x):
        self.cTime = x
    def setData(self, x):
        self.data = x

#returns array where [time, cells on border, average shape param]
def calculateData(dataArray):
    time = dataArray[0]
    cells = 0.0
    sigma = 0.0
    for i in range(1, len(dataArray)):
        if(dataArray[i] != 0):
            cells+=1.0
            sigma+=dataArray[i]
    if (cells > 0):
        return [time, cells, sigma/cells]
    else:
        return [time, 0.0, 0.0]

for parentFolder in os.listdir("C:/Users/Tom/Desktop/Research/NewDynamicShapeParamTests"):
    for childFolder in os.listdir(os.path.join("C:/Users/Tom/Desktop/Research/NewDynamicShapeParamTests", parentFolder)):
        if((childFolder != '.chaste_deletable_folder') & (childFolder != 'results_from_time_0')):
            with open(os.path.join("C:/Users/Tom/Desktop/Research/NewDynamicShapeParamTests", parentFolder, childFolder, 'shapeparam.dat'), 'r') as rawData:
                newSim = Simulation()
                for timestep in rawData:
                    formatted = []
                    formatted.append(timestep.split( ))

                    formattedData = []
                    for i in formatted:
                        for j in i:
                            if(float(j) > 0):
                                formattedData.append(float(j))
                    
                    solvedData = calculateData(formattedData)
                    borderObj = Data()
                    paramObj = Data()
                    borderObj.setTime(solvedData[0])
                    borderObj.setData(solvedData[1])
                    paramObj.setTime(solvedData[0])
                    paramObj.setData(solvedData[2])
                    newSim.addParamData(paramObj)
                    newSim.addBorderData(borderObj)
                    newSim.setGamma(float(parentFolder))
                newSim.calcAvg()
                simulationContainer.append(newSim)

#formats
head0_format = workbook.add_format({'bold' : True, 'bg_color' : '#adad85'})
head1_format = workbook.add_format({'bold' : True, 'bg_color' : '#ccccb3'})
time_format = workbook.add_format({'bg_color' : '#d1d1e0'})
border_format = workbook.add_format({'bg_color' : '#e3eae1'})
agg_format = workbook.add_format({'bold' : True, 'bg_color' : '#d0e1e1'})
param_format = workbook.add_format({'bg_color' : 'd9d9d9'})

for i in range(0, len(simulationContainer)):
    worksheet.write(0, i*4, simulationContainer[i].getGamma(), head0_format)
    worksheet.write(1, i*4, "Time", head0_format)
    worksheet.write(1, (i*4) + 1, "Border Cells", head0_format)
    worksheet.write(1, (i*4) + 2, "Mean Shape Param", head0_format)
    worksheet.write(1, (i*4) + 3, "Total Shape Param", head0_format)
    for timestep in range(0, len(simulationContainer[i].getBorderArray())):
        worksheet.write(2 + timestep, i * 4, simulationContainer[i].getBorderArray()[timestep].getTime(), time_format)
        worksheet.write(2 + timestep, (i * 4) + 1, simulationContainer[i].getBorderArray()[timestep].getData(), border_format)
        worksheet.write(2 + timestep, (i * 4) + 2, simulationContainer[i].getParamArray()[timestep].getData(), param_format)
    worksheet.write(2, (i * 4) + 3, simulationContainer[i].getAvg(), agg_format)

for i in range(0, len(simulationContainer)):
    bordersheet = workbook.add_chartsheet()
    spsheet = workbook.add_chartsheet()
    borderchart = workbook.add_chart({'type' : 'scatter'})
    spchart = workbook.add_chart({'type' : 'scatter'})
    bordersheet.set_chart(borderchart)
    spsheet.set_chart(spchart)
    borderchart.add_series({
        'categories' : ['Sheet1', 2, i * 4, simulationContainer[i].getLen(), i * 4],
        'values' : ['Sheet1', 2, (i * 4) + 1, simulationContainer[i].getLen(), (i * 4) + 1],
        'name' : f"Border Cell Count (Yc-c = {simulationContainer[i].getGamma()})",
        'marker' : {'fill' : {'color' : '#b3daff'}, 'type' : 'circle', 'border' : {'color' : '#80c1ff'}}
    })
    spchart.add_series({
        'categories' : ['Sheet1', 2, i * 4, simulationContainer[i].getLen(), i * 4],
        'values' : ['Sheet1', 2, (i * 4) + 2, simulationContainer[i].getLen(), (i * 4) + 2],
        'name' : f"Avg. Shape Parameter (Yc-c = {simulationContainer[i].getGamma()})",
        'marker' : {'fill' : {'color' : '#b3daff'}, 'type' : 'circle', 'border' : {'color' : '#80c1ff'}}
    })
    borderchart.set_style(1)
    spchart.set_style(1)
    borderchart.set_x_axis({
        'name' : 'Time (h)',
        'min' : (simulationContainer[i].getParamArray()[0].getTime() - 0.1),
        'max' : simulationContainer[i].getParamArray()[simulationContainer[i].getLen()-1].getTime()
    })
    spchart.set_x_axis({
        'name' : 'Time (h)',
        'min' : (simulationContainer[i].getParamArray()[0].getTime() - 0.1),
        'max' : simulationContainer[i].getParamArray()[simulationContainer[i].getLen()-1].getTime()
    })
    borderchart.set_y_axis({'name' : '# Of Cells On Wound Border'})
    spchart.set_y_axis({'name' : 'Mean Shape Parameter of Border Cells'})
    borderchart.set_legend({'position' : 'bottom'})
    spchart.set_legend({'position' : 'bottom'})
    borderchart.set_plotarea({'fill' : {'color' : '#f2f2f2'}})
    spchart.set_plotarea({'fill' : {'color' : '#f2f2f2'}})

workbook.close()




                    