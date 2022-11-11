import xlsxwriter
import os
import math

workbook = xlsxwriter.Workbook('915_15x15.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
boundaryFolder = r'C:\Users\Tom\Desktop\Research\9_22_15x15'
cellFolder = r'C:\Users\Tom\Desktop\Research\9_22_15x15'

num_tests = 15
num_steps = 81

CellCellSims = [] 
CellBoundSims = []
e = []

class StepData:
    def __init__(self):
        self.step = 0
        self.area = 0.0
        self.t1Swaps = 0
        self.t3Swaps = 0
        self.time = 0.0

    def getStep(self):
        return self.step
    def getTime(self):
        return self.time
    def getArea(self):
        return self.area
    def getT1(self):
        return self.t1Swaps
    def getT3(self):
        return self.t3Swaps

    def setStep(self, x):
        self.step = x
    def setTime(self, x):
        self.time = (x - 100.00)
    def setArea(self, x):
        self.area = 225.0 - x
    def setT1(self, x):
        self.t1Swaps = x
    def setT3(self, x):
        self.t3Swaps = x

class Simulation:
    #name, steps, file
    def __init__(self, n = "", s = 0, f = "", fldr = ""):
        self.name = n
        self.steps = s
        self.file = f
        self.data = []
        self.folder = fldr
        self.gamma = 0.0
        self.woundPath = f

    def initialize(self, fldr, fle, nme, stepCount):
        self.steps = stepCount
        self.name = nme
        self.file = fle
        self.folder = fldr 
        
        temp = ""
        for c in fle:
            if c.isdigit() or c == ".":
                temp += str(c)
        self.gamma = float(temp) 
        
        for c in fle:
            if c == "A":
                self.woundTime = os.path.join(self.file + r'\results_from_time_110')
            if c == "B":
                self.woundTime = os.path.join(self.file + r'\results_from_time_111')
            if c == "C":
                self.woundTime = os.path.join(self.file + r'\results_from_time_112')


        for i in range(0, stepCount):
            newStep = StepData()
            newStep.setStep(i)
            self.data.append(newStep)

        self.WoundArea()
        self.T1()
        self.T3()

    def WoundArea(self):
        with open(os.path.join(self.folder, (self.woundTime + r'\cellareas.dat')), 'r') as f:
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                file_data = []
                for i in formatted:
                    for j in i:
                        file_data.append(j)

                current_area = 0.0
                current_time = float(file_data[0])
                for i in range(5, len(file_data), 5):
                    current_area += float(file_data[i])

                for stepData in self.data:
                    if stepData.getStep() == iter:
                        stepData.setArea(current_area)
                        stepData.setTime(current_time)

                iter += 1

    def T1(self):
        with open(os.path.join(self.folder, (self.woundTime + r'\T1SwapLocations.dat')), 'r') as f:
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                file_data = []
                for i in formatted:
                    for j in i:
                        file_data.append(j)

                current_t1 = int(file_data[1])

                for stepData in self.data:
                    if stepData.getStep() == iter:
                        stepData.setT1(current_t1)

                iter += 1

    def T3(self):
        with open(os.path.join(self.folder, (self.woundTime + r'\T3SwapLocations.dat')), 'r') as f:
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                file_data = []
                for i in formatted:
                    for j in i:
                        file_data.append(j)

                current_t3 = 0
                if (iter < self.steps):
                    current_t3 = int(file_data[1])

                for stepData in self.data:
                    if stepData.getStep() == iter:
                        stepData.setT3(current_t3)

                iter += 1
    
    def getData(self):
        return self.data
    def dataAtStep(self, stp):
        return self.data[stp]
    def getTimeAtStep(self, stp):
        return self.data[stp].getTime()
    def getAreaAtStep(self, stp):
        return self.data[stp].getArea()
    def getT1AtStep(self, stp):
        return self.data[stp].getT1()
    def getT3AtStep(self, stp):
        return self.data[stp].getT3()
    def getGamma(self):
        return self.gamma

    def amendData(self, stp, x):
        self.data[stp] = x
    def setT1AtStep(self, stp, t):
        self.data[stp] = t
    def setT3AtStep(self, stp, t):
        self.data[stp] = t
    def setAreaAtStep(self, stp, a):
        self.data[stp] = a


def sortSim(array):
    tempA = array
    tempB = []

    temp = 1000.0
    position = 0
    for j in range(0, len(tempA)):
        for i in range(0, len(tempA)):
            gamma = tempA[i].getGamma()
            if gamma < temp:
                position = i
                temp = gamma
        tempB.append(tempA[position])
        tempA.pop(position)
        temp = 1000.0
        position = 0

    return(tempB)

for filename in os.listdir(cellFolder):
    if len(filename) < 5:
        newSimulation = Simulation()
        newSimulation.initialize(cellFolder, filename, "y_cell = " + str(filename), num_steps)

        CellCellSims.append(newSimulation)

for filename in os.listdir(boundaryFolder):
    if len(filename) < 5:
        newSimulation = Simulation()
        newSimulation.initialize(boundaryFolder, filename, "y_bound = " + str(filename), num_steps)

        CellBoundSims.append(newSimulation)

CellCellSims = sortSim(CellCellSims)
CellBoundSims = sortSim(CellBoundSims)

for i in range(0, len(CellCellSims)):
    print(CellCellSims[i].getGamma())

row = 3
col = 0

head1 = workbook.add_format({'bold': True, 'bg_color': '#ffd480'})
areaHead = workbook.add_format({'bold': True, 'bg_color': '#999966'})
areaData = workbook.add_format({'bg_color': '#b8b894'})
timeHead = workbook.add_format({'bold': True, 'bg_color': '#bfbfbf'})
timeData = workbook.add_format({'bg_color': '#d9d9d9'})
t1Head = workbook.add_format({'bold': True, 'bg_color': '#99b3ff'})
t1Data = workbook.add_format({'bg_color': '#ccd9ff'})
t3Head = workbook.add_format({'bold': True, 'bg_color': '#a2a2c3'})
t3Data = workbook.add_format({'bg_color': '#c2c2d6'})

worksheet.write(0, 1, 'y_cell', head1)
worksheet.write(0, 4 + (num_tests * 3), 'y_bound', head1)
worksheet.write(1, 1, 'Area', areaHead)
worksheet.write(1, 4 + (num_tests * 3), 'Area', areaHead)
worksheet.write(1, 2 + num_tests, 'T1 Swaps', t1Head)
worksheet.write(1, 5 + (num_tests * 4), 'T1 Swaps', t1Head)
worksheet.write(1, 3 + (num_tests * 2), 'T3 Swaps', t3Head)
worksheet.write(1, 6 + (num_tests * 5), 'T3 Swaps', t3Head)

for k in range(0, 6):
    worksheet.write(2, k + (k*num_tests), 'Time', timeHead)
    if k < 1: #area + time
        for cur_col in range(0, num_tests + 1):
            for cur_row in range(0, num_steps):
                if cur_col == 0:
                    worksheet.write(row + cur_row, 0, CellCellSims[0].getTimeAtStep(cur_row), timeData)
                    worksheet.write(row + cur_row, 1 + num_tests, CellCellSims[0].getTimeAtStep(cur_row), timeData)
                    worksheet.write(row + cur_row, 2 + (2 * num_tests), CellCellSims[0].getTimeAtStep(cur_row), timeData)
                    worksheet.write(row + cur_row, 3 + (3 * num_tests), CellCellSims[0].getTimeAtStep(cur_row), timeData)
                    worksheet.write(row + cur_row, 4 + (4 * num_tests), CellCellSims[0].getTimeAtStep(cur_row), timeData)
                    worksheet.write(row + cur_row, 5 + (5 * num_tests), CellCellSims[0].getTimeAtStep(cur_row), timeData)
                else:
                    worksheet.write(row - 1, cur_col, "y_" + str(CellCellSims[cur_col-1].getGamma()), areaHead)
                    worksheet.write(row + cur_row, cur_col, CellCellSims[cur_col-1].getAreaAtStep(cur_row), areaData)
                    worksheet.write(row - 1, 3 + (num_tests * 3) + cur_col, "y_" + str(CellBoundSims[cur_col-1].getGamma()), areaHead)
                    worksheet.write(row + cur_row, 3 + (num_tests * 3) + cur_col, CellBoundSims[cur_col-1].getAreaAtStep(cur_row), areaData)
    if k < 2: #t1
        for i in range(0, num_tests + 1): # col
            for j in range(0, num_steps): # row 
                if i > 0:
                    worksheet.write(row - 1, 1 + num_tests + i, "y_" + str(CellCellSims[i-1].getGamma()), t1Head)
                    worksheet.write(row - 1, 4 + (4 * num_tests) + i, "y_" + str(CellBoundSims[i-1].getGamma()), t1Head)
                    worksheet.write(row + j, 1 + num_tests + i, CellCellSims[i-1].getT1AtStep(j), t1Data)
                    worksheet.write(row + j, 4 + (4 * num_tests) + i, CellBoundSims[i-1].getT1AtStep(j), t1Data)
    else: #t3
        for i in range(0, num_tests + 1): # col
            for j in range(0, num_steps): # row 
                if i > 0:
                    worksheet.write(row - 1, 2 + (2 * num_tests) + i, "y_" + str(CellCellSims[i-1].getGamma()), t3Head)
                    worksheet.write(row - 1, 5 + (5 * num_tests) + i, "y_" + str(CellBoundSims[i-1].getGamma()), t3Head)
                    worksheet.write(row + j, 2 + (2 * num_tests) + i, CellCellSims[i-1].getT3AtStep(j), t3Data)
                    worksheet.write(row + j, 5 + (5 * num_tests) + i, CellBoundSims[i-1].getT3AtStep(j), t3Data)

def ColToText():
    alpha = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    container = alpha

    itr = 0
    for i in range(0, 10):
        for j in range(0, len(alpha)):
            container.append(str(alpha[i] + alpha[itr]))
            itr += 1
        itr = 0

    return container

e = ColToText()

#y_cell
areaSheet = workbook.add_chartsheet()
cellArea = workbook.add_chart({'type': 'line'})
cellArea.set_title({
    'name': 'Wound Area Over Time (y_cell)',
    'overlay': True
    })
for i in range(0, num_tests):
    cellArea.add_series({'values': f'=Sheet1!${e[i+1]}${row + 1}:${e[i+1]}${num_steps + row}', 'name': f'y_c = {CellCellSims[i].getGamma()}'})
areaSheet.set_chart(cellArea)
areaSheet.activate()

t1Sheet = workbook.add_chartsheet()
t1Chart = workbook.add_chart({'type': 'line'})
t1Chart.set_title({
    'name': 'T1 Swaps Over Time (y_cell)',
    'overlay': True
    })
for i in range(0, num_tests):
    t1Chart.add_series({'values': f'=Sheet1!${e[i+2+num_tests]}${row + 1}:${e[i+2+num_tests]}${num_steps + row}', 'name': f'y_c = {CellCellSims[i].getGamma()}'})
t1Sheet.set_chart(t1Chart)
t1Sheet.activate()

t3Sheet = workbook.add_chartsheet()
t3Chart = workbook.add_chart({'type': 'line'})
t3Chart.set_title({
    'name': 'T3 Swaps Over Time (y_cell)',
    'overlay': True
    })
for i in range(0, num_tests):
    t3Chart.add_series({'values': f'=Sheet1!${e[i+3+(2*num_tests)]}${row + 1}:${e[i+3+(2*num_tests)]}${num_steps + row}', 'name': f'y_c = {CellCellSims[i].getGamma()}'})
t3Sheet.set_chart(t3Chart)
t3Sheet.activate()

#y_bound
areaSheet1 = workbook.add_chartsheet()
cellArea1 = workbook.add_chart({'type': 'line'})
cellArea1.set_title({
    'name': 'Wound Area Over Time (y_boundary)',
    'overlay': True
    })
for i in range(0, num_tests):
    cellArea1.add_series({'values': f'=Sheet1!${e[i+4+(3*num_tests)]}${row + 1}:${e[i+4+(3*num_tests)]}${num_steps + row}', 'name': f'y_b = {CellBoundSims[i].getGamma()}'})
areaSheet1.set_chart(cellArea1)
areaSheet1.activate()

t1Sheet1 = workbook.add_chartsheet()
t1Chart1 = workbook.add_chart({'type': 'line'})
t1Chart1.set_title({
    'name': 'T1 Swaps Over Time (y_boundary)',
    'overlay': True
    })
for i in range(0, num_tests):
    t1Chart1.add_series({'values': f'=Sheet1!${e[i+5+(4*num_tests)]}${row + 1}:${e[i+5+(4*num_tests)]}${num_steps + row}', 'name': f'y_b = {CellBoundSims[i].getGamma()}'})
t1Sheet1.set_chart(t1Chart1)
t1Sheet1.activate()

t3Sheet1 = workbook.add_chartsheet()
t3Chart1 = workbook.add_chart({'type': 'line'})
t3Chart1.set_title({
    'name': 'T3 Swaps Over Time (y_boundary)',
    'overlay': True
    })
for i in range(0, num_tests):
    t3Chart1.add_series({'values': f'=Sheet1!${e[i+6+(5*num_tests)]}${row + 1}:${e[i+6+(5*num_tests)]}${num_steps + row}', 'name': f'y_b = {CellCellSims[i].getGamma()}'})
t3Sheet1.set_chart(t3Chart1)
t3Sheet1.activate()

workbook.close()