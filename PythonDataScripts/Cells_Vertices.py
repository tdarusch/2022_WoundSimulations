import xlsxwriter #imports library for writing new .xlsx (excel) files
import os #imports library needed for accessing files on the system

#creates a new excel file 'cell_based_data.xlsx' and adds a new worksheet
workbook = xlsxwriter.Workbook('cell_based_data.xlsx')
worksheet = workbook.add_worksheet()

path = r'C:\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0'
os.chdir(path) #changes the directory to read in from

#array containing required values from each file
dta = []

#class for formatting time, #cells, and #verts
class DataStore: 
    def __init__(self, t, nmCell, nmVert):
        self.time = t
        self.numCells = nmCell
        self.numVert = nmVert
        
        
#method for reading the file and generating new DataStores for each file
def readFile(filePath, fi):
    with open(filePath, 'r') as f:
        file_data = (f.readlines())
        fileData=[]
        i=0
        for e in range(0, len(file_data)):
            fileData.append(file_data[i].split( ))
            i+=1
        
        tme = ""
        nC = ""
        nV = ""
        for m in fi:
            if m.isdigit():
                tme = tme + m
                
        for c in fileData:
            for v in c:
                if v.startswith("NumberOfCells"):
                    for z in v:
                        if z.isdigit():
                            nC = nC + z
                    
        for d in fileData:
            for v in d:
                if v.startswith("NumberOfPoints"):
                    for z in v:
                        if z.isdigit():
                            nV = nV + z
        
        x = DataStore(tme, nC, nV)
        dta.append(x)
        
#iterates over every file in the specified path
for file in os.listdir():
    if file.endswith(".vtu"):
        fp = r'C:\Users\Tom\Desktop\Research\TestNagaiHondaMonolayer\results_from_time_0\{}'.format(file)
        
        readFile(fp, file)
       
#Starts from first cell in the 2nd row of excel worksheet      
row = 1
col = 0

#writes titles for each column
worksheet.write(0, 0, 'Time')
worksheet.write(0, 1, 'Cells')
worksheet.write(0, 2, 'Vertices')

#writes data from our read in array of data to worksheet
j = 0
for e in range(0, len(dta)):
    worksheet.write(row, col, dta[j].time)
    worksheet.write(row, col+1, dta[j].numCells)
    worksheet.write(row, col+2, dta[j].numVert)
    j += 1
    row += 1
    
workbook.close()