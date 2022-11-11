#imports necessary libraries for writing xlsx files & reading system files
import xlsxwriter
import os
import math

#creates a new excel file SimulationData with a new worksheet
workbook = xlsxwriter.Workbook('formatGamma.xlsx')
worksheet = workbook.add_worksheet()

root = r'C:\Users\Tom\Desktop\Research'
os.chdir(root)
boundaryFolder = r'C:\Users\Tom\Desktop\Research\9_15StochasticHealing'
cellFolder = r'C:\Users\Tom\Desktop\Research\9_15StochasticHealing'

steps = 251

boundary_cont = []
cellcell_cont = []

class Data:
    #constructor
    def __init__(self, g = 0):
        self.t1swaps = []
        self.areas = []
        self.t3swaps = []
        self.gamma = g

    #appends
    def append_t1swaps(self, x):
        self.t1swaps.append(x)
    def append_t3swaps(self, x):
        self.t3swaps.append(x)
    def append_areas(self, x):
        self.areas.append(x)

    #setters
    def set_gamma(self, g):
        self.gamma = g
    def set_t1swaps(self, i, v):
        self.t1swaps[i] = v
    def set_t3swaps(self, i, v):
        self.t3swaps[i] = v
    def set_areas(self, i, v):
        self.areas[i] = v

    #getters
    def get_gamma(self):
        return self.gamma
    def get_areas(self):
        return self.areas
    def get_t1swaps(self):
        return self.t1swaps
    def get_t3swaps(self):
        return self.t3swaps
    
    #individual getters
    def get_areas_val(self, i):
        return self.areas[i]
    def get_t1swaps_val(self, i):
        return self.t1swaps[i]
    def get_t3swaps_val(self, i):
        return self.t3swaps[i]

#find Data class in array
def find_data(parent_array, g_value):
    for i in range(0, len(parent_array)):
        if((parent_array[i].get_gamma()) == g_value):
            return parent_array[i]

#builds boundary container
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        new_gamma = Data()
        new_gamma.set_gamma(int(filename))
        for i in range(0, steps):
            new_gamma.append_areas(0)
            new_gamma.append_t1swaps(0)
            new_gamma.append_t3swaps(0)
        boundary_cont.append(new_gamma)

#builds cellcell container
for filename in os.listdir(cellFolder):
    if len(filename) < 3:
        new_gamma = Data()
        new_gamma.set_gamma(int(filename))
        for i in range(0, steps):
            new_gamma.append_areas(0)
            new_gamma.append_t1swaps(0)
            new_gamma.append_t3swaps(0)
        cellcell_cont.append(new_gamma)

#t1 swaps boundary
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        with open(os.path.join(boundaryFolder, (filename + r'\results_from_time_50\T1SwapLocations.dat')), 'r') as f:
            current_data = find_data(boundary_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                if (iter < steps):
                    current_data.set_t1swaps(iter, int(file_data[1]))
                    iter += 1

#t1 swaps cellcell
for filename in os.listdir(cellFolder):
    if len(filename) < 3:
        with open(os.path.join(cellFolder, (filename + r'\results_from_time_50\T1SwapLocations.dat')), 'r') as f:
            current_data = find_data(cellcell_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                if (iter < steps):
                    current_data.set_t1swaps(iter, int(file_data[1]))
                    iter += 1

#t3 swaps boundary
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        with open(os.path.join(boundaryFolder, (filename + r'\results_from_time_50\T3SwapLocations.dat')), 'r') as f:
            current_data = find_data(boundary_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                current_data.set_t3swaps(iter, int(file_data[1]))
                iter += 1

#t3 swaps cellcell
for filename in os.listdir(cellFolder):
    if len(filename) < 3:
        with open(os.path.join(cellFolder, (filename + r'\results_from_time_50\T3SwapLocations.dat')), 'r') as f:
            current_data = find_data(cellcell_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                if (iter < steps):
                    current_data.set_t3swaps(iter, int(file_data[1]))
                    iter += 1
                
#areas boundary
for filename in os.listdir(boundaryFolder):
    if len(filename) < 3:
        with open(os.path.join(boundaryFolder, (filename + r'\results_from_time_50\cellareas.dat')), 'r') as f:
            current_data = find_data(boundary_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                current_area = 0.0
                for i in range(5, len(file_data), 5):
                    current_area += float(file_data[i])

                current_data.set_areas(iter, current_area)
                iter += 1

#areas cellcell
for filename in os.listdir(cellFolder):
    if len(filename) < 3:
        with open(os.path.join(cellFolder, (filename + r'\results_from_time_50\cellareas.dat')), 'r') as f:
            current_data = find_data(cellcell_cont, int(filename))
            iter = 0
            for line in f:
                formatted = []
                formatted.append(line.split( ))
                
                file_data = []
                
                for i in formatted:
                    for j in i:
                        file_data.append(j)
                
                current_area = 0.0
                for i in range(5, len(file_data), 5):
                    current_area += float(file_data[i])

                if (iter < steps):
                    current_data.set_areas(iter, current_area)
                    iter += 1

#write to spreadsheet
row = 1
col = 0

worksheet.write(0,0, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 0, (50 + (.04 * i)))
for i in range(1, len(boundary_cont)):
    worksheet.write(0, i, 'Gamma (Boundary) = ' + str(boundary_cont[i-1].get_gamma()) + ' [Area]')
    for v in range(0, steps):
        worksheet.write(v+1, i, (99.85 - boundary_cont[i].get_areas()[v]))

#20
worksheet.write(0,20, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 20, (50 + (.04 * i)))
for i in range(1, len(cellcell_cont)):
    worksheet.write(0, i + 20, 'Gamma (Cell) = ' + str(cellcell_cont[i-1].get_gamma()) + ' [Area]')
    for v in range(0, steps):
        worksheet.write(v+1, i + 20, (99.85 - cellcell_cont[i].get_areas()[v]))

#38
worksheet.write(0, 38, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 38, (50 + (.04 * i)))
for i in range(1, len(boundary_cont)):
    worksheet.write(0, i+38, 'Gamma (Boundary) = ' + str(boundary_cont[i-1].get_gamma()) + ' [T1 Swaps]')
    for v in range(0, steps):
        worksheet.write(v+1, i+38, (boundary_cont[i].get_t1swaps()[v]))

#58
worksheet.write(0, 58, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 58, (50 + (.04 * i)))
for i in range(1, len(cellcell_cont)):
    worksheet.write(0, i+58, 'Gamma (Cell) = ' + str(cellcell_cont[i-1].get_gamma()) + ' [T1 Swaps]')
    for v in range(0, steps):
        worksheet.write(v+1, i+58, (cellcell_cont[i].get_t1swaps()[v]))

#76
worksheet.write(0, 76, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 76, (50 + (.04 * i)))
for i in range(1, len(boundary_cont)):
    worksheet.write(0, i+76, 'Gamma (Boundary) = ' + str(boundary_cont[i-1].get_gamma()) + ' [T3 Swaps]')
    for v in range(0, steps):
        worksheet.write(v+1, i+76, (boundary_cont[i].get_t3swaps()[v]))

#96
worksheet.write(0, 96, 'Time')
for i in range(0, steps):
    worksheet.write(i+1, 96, (50 + (.04 * i)))
for i in range(1, len(cellcell_cont)):
    worksheet.write(0, i+96, 'Gamma (Cell) = ' + str(cellcell_cont[i-1].get_gamma()) + ' [T3 Swaps]')
    for v in range(0, steps):
        worksheet.write(v+1, i+96, (cellcell_cont[i].get_t3swaps()[v]))

workbook.close()

#for i in range(0, len(cellcell_cont)):
#    data = cellcell_cont[i].get_areas()
#    for v in range(0, len(data)):
#        print(data[v])