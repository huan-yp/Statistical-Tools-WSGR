from msilib.schema import File
from pickle import NONE
from Req import *
def WriteMatrixAsArgs(Matrix):
    WriteArgs(str(len(Matrix))+" "+str(len(Matrix[0]))+"\n")
    for i in range(len(Matrix)):
        x = Matrix[i]
        for j in range(len(x)):
            y = x[j]
            WriteArgs(str(y)+" ")
        WriteArgs("\n")      
def WriteArgs(x,FileName = None):
    if(FileName == None):
        FileName = RD.TUNNELPATH + "\\Args.in"
    #Add str x to file File,without "\n"
    if(os.path.exists(FileName) == False):
        file = open(FileName,'w')
        file.close()
    x = str(x)
    Writer = open(FileName,"a+")
    Writer.write(x)
    Writer.close()
def ClearArgs():
    Path = RD.TUNNELPATH + "\\Args.in"
    ClearFile(Path)
    