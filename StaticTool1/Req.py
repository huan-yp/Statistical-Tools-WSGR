import os
import time
import copy
import shutil
import RunningData as RD
import numpy as np
import PIL.Image as PIM
from BasicData import *
from airtest.core.settings import Settings as ST
from airtest.core.helper import G
from numpy import  array
RD.WORKPATH = os.getcwd()
RD.TUNNELPATH = RD.WORKPATH + "\\Data\\Tunnel"
RD.DATAPATH = RD.WORKPATH + "\\Data"
def UpdateScreen():
    RD.screen = G.DEVICE.snapshot(filename=None, quality=ST.SNAPSHOT_QUALITY)
def ReadFile(Path):
    #读入一个文档,返回一个字符串列表
    Reader = open(Path,"r")
    Text = Reader.read()
    list = Text.split()
    Reader.close()
    return list
def ClearFile(Path):
    try:
        os.system("del " + Path)
    except:
        return 0 
    return 1
#存储一些常用的