#[location index] [cell id] [x-pos] [y-pos] [z-pos] [cell volume/area]

import xlsxwriter #imports library for writing new .xlsx (excel) files
import os #imports library needed for accessing files on the system

#creates a new excel file 'cell_based_data.xlsx' and adds a new worksheet
workbook = xlsxwriter.Workbook('meanAreas.xlsx')
worksheet = workbook.add_worksheet()

path = r'C:\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0'
os.chdir(path) #changes the directory to read in from

globalTime = 0

#array containing required values from each file
cells = []
AggregateCells = []

#class for formatting time, #cells, and #verts
class DataStore: 
    def __init__(self, i, x, y, a, t):
        self.cellId = i
        self.xPos = x
        self.yPos = y
        self.area = a
        self.time = t
        
class Aggregate:
    def __init__(self, c, mA):
        self.cellCnt = c
        self.meanArea = mA
        
#method for reading the file and generating new DataStores for each file
with open(r'C:\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0\cellareas.dat', 'r') as f:
    file_data = (f.readlines())
    globalTime = len(file_data)
    t=0
    
    for line in file_data:
        formed=[]
        formed2=[]
        formed2.append(line.split( ))
        
        for u in formed2:
            for z in u:
                formed.append(z)
                
        for c in range(int(len(formed)/5)):
            pos = ((c * 5) + 1)
            newCell = DataStore(float(formed[pos+1]), float(formed[pos+2]), float(formed[pos+3]), float(formed[pos+4]), t)
            cells.append(newCell)
        
        t+=1
 
for i in range(0, globalTime):
    TotalAreaAtT = 0
    numCellsAtT = 0
    for j in range(0, len(cells)):
        if(cells[j].time == i):
            numCellsAtT+=1
            TotalAreaAtT+=cells[j].area
    
    if(numCellsAtT > 0):
        meanAreaAtT = (TotalAreaAtT/numCellsAtT)
        newAggregate = Aggregate(numCellsAtT, meanAreaAtT)
        AggregateCells.append(newAggregate)
    
 
#Starts from first cell in the 2nd row of excel worksheet      
row = 1
col = 0

#writes titles for each column
worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Cells')
worksheet.write(0, 2, 'Mean Area')
worksheet.write(0, 4, 'Time')
worksheet.write(0, 5, 'Cells')
worksheet.write(0, 6, 'Mean Area')

#writes data from our read in array of data to worksheet
for e in range(0, len(AggregateCells)):
    worksheet.write(row, col, e)
    worksheet.write(row, col+1, AggregateCells[e].cellCnt)
    worksheet.write(row, col+2, AggregateCells[e].meanArea)
    row += 1
    
row = 1
for e in range(0, int((int(len(AggregateCells)) + 10)/10)):
    worksheet.write(row, col+4, (e*10))
    worksheet.write(row, col+5, AggregateCells[e*10].cellCnt)
    worksheet.write(row, col+6, AggregateCells[e*10].meanArea)
    row += 1
    
workbook.close()