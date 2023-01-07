#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('SwapData.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
project = r'\Users\Tom\Desktop\Research'

#data file path declaration
pathWoundT1 = r'C:\Users\Tom\Desktop\Research\TestWoundHealing\NagaiHonda\results_from_time_50\T1SwapLocations.dat'
pathWoundT3 = r'C:\Users\Tom\Desktop\Research\TestWoundHealing\NagaiHonda\results_from_time_50\T3SwapLocations.dat'
pathMotileT1 = r'C:\Users\Tom\Desktop\Research\TestVertexCellMoving\results_from_time_30\T1SwapLocations.dat'
pathMotileT3 = r'C:\Users\Tom\Desktop\Research\TestVertexCellMoving\results_from_time_30\T3SwapLocations.dat'

DataContainer = []

class Data:
    #constructor
    def __init__(self, t = 0, wT1 = 0, wT3 = 0, mT1 = 0, mT3 = 0):
        self.Time = t
        self.woundT1 = wT1
        self.woundT3 = wT3
        self.motileT1 = mT1
        self.motileT3 = mT3
      
    def get_Time(self):
        return self.Time
        
    def get_woundT1(self):
        return self.woundT1
        
    def get_woundT3(self):
        return self.woundT3
        
    def get_motileT1(self):
        return self.motileT1
        
    def get_motileT3(self):
        return self.motileT3
    
    def set_Time(self, x):
        self.Time = x
        
    def set_woundT1(self, x):
        self.woundT1 = x
        
    def set_woundT3(self, x):
        self.woundT3 = x
        
    def set_motileT1(self, x):
        self.motileT1 = x
    
    def set_motileT3(self, x):
        self.motileT3 = x

with open(pathWoundT1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        DataContainer.append(Data())
        DataContainer[t].set_Time(t);
        t+=1;

with open(pathWoundT1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_woundT1(formatted[1])
        t += 1

with open(pathWoundT3) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_woundT3(formatted[1])
        t += 1
        
with open(pathMotileT1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_motileT1(formatted[1])
        t += 1
        
with open(pathMotileT3) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_motileT3(formatted[1])
        t += 1
        
row = 1
col = 0

worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Wound Healing T1')
worksheet.write(0, 2, 'Wound Healing T3')
worksheet.write(0, 3, 'Motile Cell T1')
worksheet.write(0, 4, 'Motile Cell T3')
    
for i in range(0, len(DataContainer)):
    worksheet.write(row, col, i)
    worksheet.write(row, col+1, int(DataContainer[i].get_woundT1()))
    worksheet.write(row, col+2, int(DataContainer[i].get_woundT3()))
    worksheet.write(row, col+3, int(DataContainer[i].get_motileT1()))
    worksheet.write(row, col+4, int(DataContainer[i].get_motileT3()))
    row += 1
    
workbook.close()