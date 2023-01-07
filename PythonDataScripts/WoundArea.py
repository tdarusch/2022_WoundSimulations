#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('SimulationData.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research\TestWoundHealing\NagaiHonda\results_from_time_50'
os.chdir(root)
project = r'\Users\Tom\Desktop\Research\TestWoundHealing\NagaiHonda\results_from_time_50'

#data file path declaration
pathAreas = os.path.join(project, 'cellareas.dat')

FullArea = 99.862466
DataContainer = []

class AreaData:
    #constructor
    def __init__(self, t = 0, a = 0):
        self.Time = t
        self.Area = a
        
    def get_Area(self):
        return self.Area
        
    def get_Time(self):
        return self.Time
        
    def get_AreaDiff(self):
        return (FullArea-self.Area)
        
    def set_Area(self, x):
        self.Area = x
        
    def set_Time(self, x):
        self.Time = x

with open(pathAreas) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        DataContainer.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        print(totalArea)
            
        DataContainer[t].set_Time(t)
        DataContainer[t].set_Area(totalArea)
        t += 1
        
row = 1
col = 0

worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Wound Area')
    
for i in range(0, len(DataContainer)):
    worksheet.write(row, col, i)
    worksheet.write(row, col+1, DataContainer[i].get_AreaDiff())
    row += 1
    
workbook.close()