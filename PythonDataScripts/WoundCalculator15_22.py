#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('Sim15_22WoundArea.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
project = r'\Users\Tom\Desktop\Research'

#data file path declaration
pathSim15 = r'\Users\Tom\Desktop\Research\WoundHealSim15\results_from_time_50\cellareas.dat'
pathSim16 = r'\Users\Tom\Desktop\Research\WoundHealSim16\results_from_time_50\cellareas.dat'
pathSim17 = r'\Users\Tom\Desktop\Research\WoundHealSim17\results_from_time_50\cellareas.dat'
pathSim18 = r'\Users\Tom\Desktop\Research\WoundHealSim18\results_from_time_50\cellareas.dat'
pathSim19 = r'\Users\Tom\Desktop\Research\WoundHealSim19\results_from_time_50\cellareas.dat'
pathSim20 = r'\Users\Tom\Desktop\Research\WoundHealSim20\results_from_time_50\cellareas.dat'
pathSim21 = r'\Users\Tom\Desktop\Research\WoundHealSim21\results_from_time_50\cellareas.dat'
pathSim22 = r'\Users\Tom\Desktop\Research\WoundHealSim22\results_from_time_50\cellareas.dat'

FullArea = 100

Sim15Container = []
Sim16Container = []
Sim17Container = []
Sim18Container = []
Sim19Container = []
Sim20Container = []
Sim21Container = []
Sim22Container = []

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

with open(pathSim15) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim15Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
        
        Sim15Container[t].set_Time(t)
        Sim15Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim16) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim16Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim16Container[t].set_Time(t)
        Sim16Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim17) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim17Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim17Container[t].set_Time(t)
        Sim17Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim18) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim18Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim18Container[t].set_Time(t)
        Sim18Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim19) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim19Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim19Container[t].set_Time(t)
        Sim19Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim20) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim20Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim20Container[t].set_Time(t)
        Sim20Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim21) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim21Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim21Container[t].set_Time(t)
        Sim21Container[t].set_Area(totalArea)
        t += 1
        
with open(pathSim22) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        Sim22Container.append(AreaData())
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        totalArea = 0.0
        for i in range(5, len(formatted), 5):
            totalArea += float(formatted[i])
            
        Sim22Container[t].set_Time(t)
        Sim22Container[t].set_Area(totalArea)
        t += 1
        
row = 1
col = 0

worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Sim 15 Wound Area')
worksheet.write(0, 2, 'Sim 16 Wound Area')
worksheet.write(0, 3, 'Sim 17 Wound Area')
worksheet.write(0, 4, 'Sim 18 Wound Area')
worksheet.write(0, 5, 'Sim 19 Wound Area')
worksheet.write(0, 6, 'Sim 20 Wound Area')
worksheet.write(0, 7, 'Sim 21 Wound Area')
worksheet.write(0, 8, 'Sim 22 Wound Area')

    
for i in range(0, len(Sim15Container)):
    worksheet.write(row, col, i)
    worksheet.write(row, col+1, Sim15Container[i].get_AreaDiff())
    worksheet.write(row, col+2, Sim16Container[i].get_AreaDiff())
    worksheet.write(row, col+3, Sim17Container[i].get_AreaDiff())
    worksheet.write(row, col+4, Sim18Container[i].get_AreaDiff())
    worksheet.write(row, col+5, Sim19Container[i].get_AreaDiff())
    worksheet.write(row, col+6, Sim20Container[i].get_AreaDiff())
    if i < len(Sim21Container):
        worksheet.write(row, col+7, Sim21Container[i].get_AreaDiff())
    else:
        worksheet.write(row, col+7, 0)
    worksheet.write(row, col+8, Sim22Container[i].get_AreaDiff())
    row += 1
    
workbook.close()