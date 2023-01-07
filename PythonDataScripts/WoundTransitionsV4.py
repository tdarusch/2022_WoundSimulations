#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('SwapDataV4.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
project = r'\Users\Tom\Desktop\Research'

#data file path declaration
pathSim4T1 = r'C:\Users\Tom\Desktop\Research\WoundHealSim7\results_from_time_50\T1SwapLocations.dat'
pathSim4T3 = r'C:\Users\Tom\Desktop\Research\WoundHealSim7\results_from_time_50\T3SwapLocations.dat'
pathSim5T1 = r'C:\Users\Tom\Desktop\Research\WoundHealSim8\results_from_time_50\T1SwapLocations.dat'
pathSim5T3 = r'C:\Users\Tom\Desktop\Research\WoundHealSim8\results_from_time_50\T3SwapLocations.dat'

DataContainer = []

class Data:
    #constructor
    def __init__(self, t = 0, s4T1 = 0, s4T3 = 0, s5T1 = 0, s5T3 = 0, s6T1 = 0, s6T3 = 0):
        self.Time = t
        self.sim4T1 = s4T1;
        self.sim4T3 = s4T3;
        self.sim5T1 = s5T1;
        self.sim5T3 = s5T3;
        self.sim6T1 = s6T1;
        self.sim6T3 = s6T3;
      
    def get_Time(self):
        return self.Time
    
    def get_sim4T1(self):
        return self.sim4T1;
        
    def get_sim4T3(self):
        return self.sim4T3;
        
    def get_sim5T1(self):
        return self.sim5T1;
    
    def get_sim5T3(self):
        return self.sim5T3;

           
    def set_time(self, x):
        self.time = x;
        
    def set_sim4T1(self, x):
        self.sim4T1 = x;
        
    def set_sim4T3(self, x):
        self.sim4T3 = x;
        
    def set_sim5T1(self, x):
        self.sim5T1 = x;
        
    def set_sim5T3(self, x):
        self.sim5T3 = x;
        

with open(pathSim4T1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        DataContainer.append(Data())
        DataContainer[t].set_time(t);
        t+=1;

with open(pathSim4T1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_sim4T1(formatted[1])
        t += 1
        
with open(pathSim4T3) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_sim4T3(formatted[1])
        t += 1
        
with open(pathSim5T1) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_sim5T1(formatted[1])
        t += 1
        
with open(pathSim5T3) as file:
    file_data = file.readlines()
    
    t=0
    for line in file_data:
        format1 = []
        format1.append(line.split( ))
        
        formatted = []
        
        for i in format1:
            for j in i:
                formatted.append(j)
                
        DataContainer[t].set_sim5T3(formatted[1])
        t += 1
        


row = 1
col = 0

worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, '(T1) Sim 13')
worksheet.write(0, 2, '(T1) Sim 14')
worksheet.write(0, 3, '(T3) Sim 13')
worksheet.write(0, 4, '(T3) Sim 14')
    
for i in range(0, len(DataContainer)):
    worksheet.write(row, col, i)
    worksheet.write(row, col+1, int(DataContainer[i].get_sim4T1()))
    worksheet.write(row, col+2, int(DataContainer[i].get_sim5T1()))
    worksheet.write(row, col+3, int(DataContainer[i].get_sim4T3()))
    worksheet.write(row, col+4, int(DataContainer[i].get_sim5T3()))
    row += 1
    
workbook.close()