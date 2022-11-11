#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('SimulationData.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0'
os.chdir(root)
project = r'\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0'

#data file path declaration
pathAges = os.path.join(project, 'cellages.dat')
pathAreas = os.path.join(project, 'cellareas.dat')
pathProlifTypes = os.path.join(project, 'results.vizprolifcelltype')
pathMutations = os.path.join(project, 'results.vizmutationstates')
pathLabels = os.path.join(project, 'results.vizlabels')
pathPhases = os.path.join(project, 'results.vizcellphases')

#FORMATS
#Labels: |cell-label|index|xpos|ypos| *label = 0 or label = 5
#Ages: dT |index|xpos|ypos|age|
#Phases: dT |phase|
#Areas: dT |location-index|cell-id|xpos|ypos|zpos|cell-area|

dataContainer = [] #contains a Data object at each index corresponding to t, containing all aggregate value at the corresponding timestep
cells = [] #contains a Cell object for each cell at t=endTime, in turn contains each respective cell's distance from the mesh center, its area, and whether it is mitotic
ranges = [] #populated with Span objects, which correspond to equally spaced gaps between [minimum distance from center, maximum distance from center]; 
#each Span contains the mean area of cells and the number of mitotic cells that fall within its corresponding range of distances
chartsheets = [] #contains chart sheets

#creates 8 workbooks
for i in range(0, 9):
    chartsheets.append(workbook.add_chartsheet())

#Parameters: [INDEX|AGE|AREA|PROLIFTYPE|MUTATIONSTATE|XPOSITION|YPOSITION|PHASE(BOOL)|TIME]
#class containing aggregate data for each timestep
class Data:
    #constructor
    def __init__(self, nc = 0, nv = 0, ar = 0, mc = 0, ni = 0, md = 0):
        self.meanArea = ar
        self.mitoticCount = mc
        self.numCells = nc
        self.numVertices = nv
        self.numInhibited = ni
        self.meanCenterDistance = md
    
    #getters
    def get_meanArea(self):
        return self.meanArea
        
    def get_meanCenterDistance(self):
        return self.meanCenterDistance
        
    def get_mitoticCount(self):
        return self.mitoticCount
        
    def get_numCells(self):
        return self.numCells
        
    def get_numVertices(self):
        return self.numVertices
        
    def get_numInhibited(self):
        return self.numInhibited
        
    #setters
    def set_meanArea(self, x):
        self.meanArea = x
        
    def set_meanCenterDistance(self, x):
        self.meanCenterDistance = x
        
    def set_mitoticCount(self, x):
        self.mitoticCount = x
        
    def set_numCells(self, x):
        self.numCells = x
        
    def set_numVertices(self, x):
        self.numVertices = x
        
    def set_numInhibited(self, x):
        self.numInhibited = x

#Class for containing area, mitotic state, index, and position for each cell in the mesh at t = endTime
class Cell:
    def __init__(self, a = 0, m = False, i = 0, xi = 0.1, y = 0.1):
        self.area = a
        self.mitotic = m
        self.index = i
        self.xPos = xi
        self.yPos = y
    
    #helper methods
    
    #calculates a cell's distance from (0,0) by the formula x = sqrt((x-x_origin)^2 + (y-y_origin)^2)
    def get_distanceFromCenter(self):
        distance = 0.0
        distance += pow(self.xPos, 2)
        distance += pow(self.yPos, 2)
        calDist = math.sqrt(distance)
        return calDist
        
    #getters
    def get_area(self):
        return self.area
        
    def get_mitotic(self):
        return self.mitotic
        
    def get_index(self):
        return self.index
        
    def get_xPos(self):
        return self.xPos
        
    def get_yPos(self):
        return self.yPos
        
    #setters        
    def set_area(self, x):
        self.area = x
        
    def set_mitotic(self, x):
        self.mitotic = x
        
    def set_index(self, x):
        self.index = x
        
    def set_xPos(self, x):
        self.xPos = x
        
    def set_yPos(self, x):
        self.yPos = x
        
#Class containing equally spaced blocks of distances from mesh center ranging from [0, max distance from center]
#Each range block contains a variable for the number of mitotic cells and the mean areas of cells x distance from
#the mesh center;
#each of these variables is populated via a number of helper methods
class Span:
    def __init__(self, r = 0, ta = 0.0, nc = 0, nm = 0):
        self.span = r
        self.totalArea = ta
        self.numCells = nc
        self.numMitotic = nm
    
    #helper methods
    
    #iterates the number of mitotic cells when called
    def iterateMitotic(self):
        self.numMitotic += 1
        
    #appends a parameter x to the totalArea variable, which is used to calculate mean area
    #iterates number of cells when called to simplify function, as each cell will have its
    #area appended eventually
    def appendArea(self, x):
        self.totalArea = self.totalArea + x
        self.numCells = self.numCells + 1
        
    #calculates mean area by totalArea/#cells
    def meanArea(self):
        mean = self.totalArea/self.numCells
        return mean
        
    #getters
    def get_range(self):
        return self.span
        
    def get_totalArea(self):
        return self.totalArea
        
    def get_numCells(self):
        return self.numCells
        
    def get_numMitotic(self):
        return self.numMitotic
        
    #setters
    def set_range(self, x):
        self.span = x
        
    def set_totalArea(self, x):
        self.totalArea = x
    
    def set_numCells(self, x):
        self.numCells = x
        
    def set_numMitotic(self, x):
        self.numMitotic = x
    
#Create dataContainer Array, whose indices hold the aggregate data corresponding to the equivalent timestep
#and populates it with t number of Data objects
with open(pathLabels) as file:
    file_data = file.readlines()
        
    i = 0
    for line in file_data:
        dataContainer.append(Data())
        i += 1

#Inhibition Sequence
#Sets the number of inhibited cells for each Data object in dataContainer 
with open(pathLabels) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        nInhibited = 0
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
        
        for i in range(1, len(formatted) - 1, 4):
            if(int(formatted[i]) == 5):
                nInhibited += 1
        
        dataContainer[t].set_numInhibited(nInhibited)
        t += 1
        
#Area Sequence & Cell count & mean Distance
#Sets the mean area of cells for each Data object in dataContainer 
#Sets the number of cells for each Data object in dataContainer 
#Sets the mean distance from center for each Data object in dataContainer 
with open(pathAreas) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        mArea = 0.0
        mDistance = 0.0
        xPos = []
        yPos = []
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
        
        n = 0.0
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            n += 1

        for i in range(3, len(formatted), 5):
            xPos.append(float(formatted[i]))
            
        for i in range(4, len(formatted), 5):
            yPos.append(float(formatted[i]))
            
        totalDistance = 0.0
        n1 = 0.0
        
        for i in range(0, len(xPos)):
            x = 0
            x += pow(xPos[i], 2)
            x += pow(yPos[i], 2)
            totalDistance += math.sqrt(x)
            n1 += 1
        
        if n1>0:
            mDistance = totalDistance/n1
        
        if n>0:
            mArea = (totalArea/n)
        
        dataContainer[t].set_meanCenterDistance(mDistance)        
        dataContainer[t].set_meanArea(mArea)
        dataContainer[t].set_numCells(n)
        t += 1
                
#Mitotic Count
#Sets the number of mitotic cells for each Data object in dataContainer 
with open(pathPhases) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        nMitotic = 0
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
        
        for i in range(1, len(formatted)):
            if int(formatted[i]) == 4:
                nMitotic += 1
        
        dataContainer[t].set_mitoticCount(nMitotic)
        t += 1
        
#Vertex Count Method
#Sets the number of vertices for each Data object in dataContainer
#Opens a passed results_t.vtu and iterates through xml until it reaches "NumberOfPoints"
def getVertices(filePath, fi, t):
    with open(filePath, 'r') as f:
        file_data = (f.readlines())
        fileData=[]
        i=0
        for e in range(0, len(file_data)):
            fileData.append(file_data[i].split( ))
            i+=1
        
        nV = ""
        for d in fileData:
            for v in d:
                if v.startswith("NumberOfPoints"):
                    for z in v:
                        if z.isdigit():
                            nV = nV + z
                                    
        dataContainer[t].set_numVertices(int(nV))
        
#iterates through all files in the test directory and calls getVertices(file) for each
#results_t.vtu
for file in os.listdir():
    if file.endswith(".vtu"):
        fp = os.path.join(project, file)
        t = ''.join(x for x in file if x.isdigit())
        if int(t)>0:
            t = int(t)/1000
        getVertices(fp, file, int(t))

#Distance from center vs mean area
#Sets the x-position, y-position, and appends total area for each cell object in cells
with open(pathAreas) as file:
    file_data = file.readlines()
    
    t=0
    for line in range(0, len(file_data)):
        if line == (len(file_data) - 1):
            format1 = []
            format1.append(file_data[line].split( ))
        
            formatted = []
            
            for i in format1:
                for j in i:
                    formatted.append(j)
                    
            for i in range(2, len(formatted), 5):
                found = False
                index = 0
                for j in range(0, len(cells)):
                    if cells[j].get_index == int(formatted[i]):
                        found = True
                        index = j
                if found:
                    cells[index].set_xPos(float(formatted[i+1]))
                    cells[index].set_yPos(float(formatted[i+2]))
                    cells[index].set_area(float(formatted[i+3]))
                else:
                    newCell = Cell()
                    newCell.set_index(float(formatted[i]))
                    newCell.set_xPos(float(formatted[i+1]))
                    newCell.set_yPos(float(formatted[i+2]))
                    newCell.set_area(float(formatted[i+3]))
                    cells.append(newCell)

#Distance from center vs mitotic
#Sets the boolean mitotic to true if a given cell at t=endTime is in phase 4 of its cell cycle
with open(pathPhases) as file:
    file_data = file.readlines()
    
    for line in range(0, len(file_data)):
        if line == (len(file_data) - 1):
            format1 = []
            format1.append(file_data[line].split( ))
            
            formatted = []
            
            for i in format1:
                for j in i:
                    formatted.append(j)
            
            for i in range(1, len(formatted)):
                if int(formatted[i]) == 4:
                    found = False
                    index = 0
                    for j in range(0, len(cells)):
                        if cells[j].get_index() == i:
                            found = True
                            index = j
                    if found:
                        cells[index].set_mitotic(True)
#                    else:
#                        print("UNFOUND")
#                        newCell = Cell()
#                        newCell.set_mitotic(True)
#                        cells.append(newCell)

#find the maximum distance from center to determine blocking for graphs
maxDist = 0.0
for i in range(0, len(cells)):
    if(cells[i].get_distanceFromCenter() > maxDist):
        maxDist = cells[i].get_distanceFromCenter()

#generates new distance from center ranges as a function of maxDist/10
maxDist = math.ceil(maxDist)
iterator = int(maxDist/10)
for j in range(0, maxDist, int(maxDist/10)):
    newBlock = Span()
    newBlock.set_range(int(iterator))
    ranges.append(newBlock)
    iterator += int(maxDist/10)
    
#checks mitotic count for each cell and iterates the number of mitotic cells at ranges[cell distance from center] by 1
for i in range(0, len(cells)):
    dist = math.floor(cells[i].get_distanceFromCenter())
    for j in range(0, len(ranges)):
        if dist < ranges[j].get_range():
            ranges[j].appendArea(cells[i].get_area())
            if(cells[i].get_mitotic() == True):
                ranges[j].iterateMitotic()
            break

#Starts from first cell in 2nd row of excel worksheet
rowGlob = 1
colGlob = 0

#Writes aggregate data column titles
worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Cells')
worksheet.write(0, 2, 'Vertices')
worksheet.write(0, 3, 'Mean Cell Distance From Center')
worksheet.write(0, 4, 'Mitotic Cells')
worksheet.write(0, 5, 'Inhibited Cells')
worksheet.write(0, 6, 'sqrt(NumberOfCells)')

#Writes distance from center vs inhibited cells & distance from center & mean area titles
worksheet.write(0, 7, 'Distance From Center')
worksheet.write(0, 8, 'Mean Cell Area')
worksheet.write(0, 9, 'Mitotic')
worksheet.write(0, 10, 'Fraction of Mitotic Cells')

#FORMAT
#time|cells|vertices|meanDist|mitoticCells|inhibitedCells| null |distanceFromCenter|meanArea|inhibitedCells|frac inhibitedCells|

#generates time|cells|vertices|meanDist|mitoticCells|inhibitedCells| 
row = rowGlob
col = colGlob
for i in range(0, len(dataContainer)):
    worksheet.write(row, col, i)
    worksheet.write(row, col+1, dataContainer[i].get_numCells())
    worksheet.write(row, col+2, dataContainer[i].get_numVertices())
    worksheet.write(row, col+3, dataContainer[i].get_meanCenterDistance())
    worksheet.write(row, col+4, dataContainer[i].get_mitoticCount())
    worksheet.write(row, col+5, dataContainer[i].get_numInhibited())
    worksheet.write(row, col+6, math.sqrt(dataContainer[i].get_numCells()))
    row += 1
    
#generates distanceFromCenter|meanArea|inhbitiedCells|frac InhibitedCells
row = rowGlob
col = colGlob
for i in range(0, len(ranges)):
    if i == 0:
        x = str(0) + " - " + str(ranges[0].get_range())
        worksheet.write(row, col+7, x)
    else:
        x = str(ranges[i-1].get_range()) + " - " + str(ranges[i].get_range())
        worksheet.write(row, col+7, x)
        worksheet.write(row, col+8, ranges[i].meanArea())
        worksheet.write(row, col+9, ranges[i].get_numMitotic())
        worksheet.write(row, col+10, (ranges[i].get_numMitotic()/ranges[i].get_numCells()))
        row += 1

#formats titles
cell_title = workbook.add_format()
cell_title.set_bold()
cell_title.set_align('center')
worksheet.set_row(0, 11, cell_title)


currentChart = 0
#params
#title|x_label|y_label|value_col|value_row_start|value_row_end|category_col|category_row_start|category_row_end
def newColumnChart(title, x_label, y_label, value_col, value_row_start, value_row_end, category_col, category_row_start, category_row_end):
    global currentChart
    chart1 = workbook.add_chart({'type': 'column'})
    chart1.add_series({
        'values': '=Sheet1!$' + str(value_col) + '$' + str(value_row_start) +':$' + str(value_col) + '$' + str(value_row_end),
        'categories': '=Sheet1!$' + str(category_col) + '$' + str(category_row_start) +':$' + str(category_col) + '$' + str(category_row_end),
        'fill': {'color': 'gray'},
        'border': {'color': 'black'}    
    })
    chart1.set_title({
        'name': str(title),
        'overlay': 'True',
    })
    chart1.set_legend({'none': True})
    chart1.set_x_axis({'name': str(x_label)})
    chart1.set_y_axis({'name': str(y_label)})
    
    chartsheets[currentChart].set_chart(chart1)
    currentChart+=1
    
def newLineChart(title, x_label, y_label, value_col, value_row_start, value_row_end, category_col, category_row_start, category_row_end, trend_type):
    global currentChart
    chart1 = workbook.add_chart({'type': 'line'})
    if trend_type == 'none':
        chart1.add_series({
            'values': '=Sheet1!$' + str(value_col) + '$' + str(value_row_start) +':$' + str(value_col) + '$' + str(value_row_end),
            'categories': '=Sheet1!$' + str(category_col) + '$' + str(category_row_start) +':$' + str(category_col) + '$' + str(category_row_end),
            'fill': {'color': 'gray'},
            'border': {'color': 'black'},
            #'marker': {
            #    'type': 'circle',
            #    'size': .5,
            #    'border': {'color': 'black'},
            #    'fill': {'color': 'gray'}
            #    },
            'line': {
                'none': False,
                'fill': {'color': 'gray'},
                'border': {'color': 'black'}
            }
        })
    else:
        chart1.add_series({
            'values': '=Sheet1!$' + str(value_col) + '$' + str(value_row_start) +':$' + str(value_col) + '$' + str(value_row_end),
            'categories': '=Sheet1!$' + str(category_col) + '$' + str(category_row_start) +':$' + str(category_col) + '$' + str(category_row_end),
            'fill': {'color': 'gray'},
            'border': {'color': 'black'},
            #'marker': {
            #    'type': 'circle',
            #    'size': .5,
            #    'border': {'color': 'black'},
            #    'fill': {'color': 'gray'}
            #    },
            'line': {
                'none': False,
                'fill': {'color': 'gray'},
                'border': {'color': 'black'}
            },
            'trendline': {'type': trend_type}
        })
    chart1.set_title({
        'name': str(title),
        'overlay': 'True',
    })
    chart1.set_legend({'none': True})
    chart1.set_x_axis({'name': str(x_label)})
    chart1.set_y_axis({'name': str(y_label)})
    
    chartsheets[currentChart].set_chart(chart1)
    currentChart+=1



#dist from center v meanArea histogram
newColumnChart(
    'Distance From Center v. Mean Cell Area',
    'Distance from center',
    'Mean area',
    'I',
    '2',
    '12',
    'H',
    '2',
    '12'
)

#dist from center v num mitotic
newColumnChart(
    'Distance From Center v. Mitotic Cell Count',
    'Distance from center',
    'Mitotic cell count',
    'J',
    '2',
    '12',
    'H',
    '2',
    '12'
)

#dist from center v mitotic fraction
newColumnChart(
    'Distance From Center v. Fractional Mitotic Cell Count',
    'Distance from center',
    'Fractional mitotic cell count',
    'K',
    '2',
    '12',
    'H',
    '2',
    '12'
)

#number of miotic cells at time t
newColumnChart(
    'Miotic Cell Count',
    'Time',
    'Miotic cells',
    'E',
    '2',
    '177',
    'A',
    '2',
    '177'
)

#number of contact inhibited cells at time t
newColumnChart(
    'Contact Inhibited Cells',
    'Time',
    'Contact inhibited cells',
    'F',
    '2',
    '177',
    'A',
    '2',
    '177'
)

#cell count over time
newLineChart(
    'Cell Count Over Time - Exponential Fit',
    'Time',
    'Cells',
    'B',
    '2',
    '177',
    'A',
    '2',
    '177',
    'exponential'
)

#vertex count over time
newLineChart(
    'Vertex Count Over Time - Exponential Fit',
    'Time',
    'Vertices',
    'C',
    '2',
    '177',
    'A',
    '2',
    '177',
    'exponential'
)

#mean distance over time
newLineChart(
    'Mean Distance From Mesh Center - Exponential Fit',
    'Time',
    'Mean distance from mesh center',
    'D',
    '2',
    '177',
    'A',
    '2',
    '177',
    'exponential'
)

#sqrt(numCells)
newLineChart(
    'sqrt(Number Of Cells) - Linear Fit',
    'sqrt(number of cells)',
    'Time',
    'G',
    '2',
    '177',
    'A',
    '2',
    '177',
    'linear'
)


#dfc_meanAreaChart = workbook.add_chart({'type': 'column'})
#dfc_meanAreaChart.add_series({
#    'values': '=Sheet1!$I$2:$I$12',
#    'categories': '=Sheet1!$H$2:$H$12',
#    'fill': {'color': 'gray'},
#    'border': {'color': 'black'}    
#})
#dfc_meanAreaChart.set_title({
#    'name': 'Distance From Center v. Mean Cell Area',
#    'overlay': 'False'
#})
#dfc_meanAreaChart.set_legend({'none': True})
#dfc_meanAreaChart.set_x_axis({'name': 'Distance from center'})
#dfc_meanAreaChart.set_y_axis({'name': 'Mean area'})


workbook.close()
#tester print loops
#for i in range(0, len(ranges)):
#    print(ranges[i].get_numCells())
#for i in range(0, len(cells)):
#     print("| Mitotic: " + str(cells[i].get_mitotic()) + "| Area: " + str(cells[i].get_area()) + "| Center Dist: " + str(cells[i].get_distanceFromCenter()))
#for i in range(0, len(ranges)):
#    print("Range: " + str(ranges[i].get_range()) + " | Mitotic: " + str(ranges[i].get_numMitotic()) + "| Mean area: " + str(ranges[i].meanArea()))