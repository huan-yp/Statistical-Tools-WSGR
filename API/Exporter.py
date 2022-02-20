import os
import time
import sys
RequirementPath = os.path.dirname(__file__)
RecognizerPath = RequirementPath + "\\VideoRecognizer.exe"
def RecognizeVideo(VideoPath:str,FightGap:float):
    InitialDir = os.getcwd()
    os.chdir(RequirementPath)
    os.system("del data.in")
    with open(RequirementPath+"\\data.in",mode="a+") as file:
        file.write(VideoPath+"\n")
        file.write(str(FightGap)+"\n")
    try:
        os.system(RecognizerPath)
    except:
        print("Unexception C++ Error!")
    with open(RequirementPath+"\\data.out",mode="r") as file:
        res = file.read()
    os.chdir(InitialDir)
    return res
def CheckPath():
    print("os.getcwd():",os.getcwd())
    print("__file__:",__file__)
    print("RequirementPath:",RequirementPath)
    print("RecognizerPath:",RecognizerPath)
    print("sys.path:",sys.path)