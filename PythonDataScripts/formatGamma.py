#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('SwapData.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
boundaryFolder = r'C:\Users\Tom\Desktop\Research\ModifiedWoundHealingCellBoundary'
cellFolder = r'C:\Users\Tom\Desktop\Research\ModifiedWoundHealingCellCell'


boundary_container = []
cellcell_container = []

class Data_Container:
    #constructor
    def __init__(self, g = 0):
        self.gamma = g
        self.data = []

    #getters
    def get_gamma(self):
        return self.gamma
    
    def get_data_at(self, x):
        return self.data[x]
    
    #setters
    def set_gamma(self, x):
        gamma = x
    def set_data_at(self, x, y):
        self.data[x] = y
    
    def append_data(self, x):
        self.data.append(x)

class Data:
    #constructor
    def __init__(self, t = 0, t1s = 0, t3s = 0, a = 0):
        self.time = t
        self.t1 = t1s
        self.t3 = t3s
        self.area = a

    #getters
    def get_time(self):
        return self.time
    def get_t1(self):
        return self.t1
    def get_t3(self):
        return self.t3
    def get_area(self):
        return self.area
    def get_wound_area(self):
        return (100 - self.area)

    #setters
    def set_time(self, x):
        self.time = x
    def set_t1(self, x):
        self.t1 = x
    def set_t3(self, x):
        self.t3 = x
    def set_area(self, x):
        self.area = x



#t1 swaps boundary
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        with open(os.path.join(boundaryFolder, (filename + r'\results_from_time_50\T1SwapLocations.dat')), 'r') as f:
            new_data = Data_Container()
            new_data.set_gamma(filename)
            #populate data array
            for i in range(0, 251):
                ti = (50.0 + (i * 0.04))
                time_step = Data()
                time_step.set_time(ti)
                new_data.append_data(time_step)
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                current_data = new_data.get_data_at(iter)
                current_data.set_t1(file_data[1])
                iter += 1
            boundary_container.append(new_data)



#t3 swaps boundary
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        with open(os.path.join(boundaryFolder, (filename + r'\results_from_time_50\T3SwapLocations.dat')), 'r') as f:
            new_data = boundary_container[itr]
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                current_data = new_data.get_data_at(iter)
                current_data.set_t3(file_data[1])
                iter += 1

  
for i in range(0, len(boundary_container)):
    print(boundary_container[i].get_gamma())