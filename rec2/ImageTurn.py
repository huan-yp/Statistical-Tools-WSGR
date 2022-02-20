from PIL import Image
from numpy import array
import os
import time
from MathFunction import *
from ConstVariables import *
def GetKeyPoint(path1,path2,line:Line,Limit = 7,Type = "MISS"):

    #传入路径 path1，按照渐变色二值化法变换后存入 path2
    #计算与色彩回归直线的距离，判断是否为关键点
    img = Image.open(path1)
    arr = array(img)
    for i,row in enumerate(arr):
        for j,color in enumerate(row):
            dis = 1e9
            if(Type == "MISS"):
                
                if(not (color[0] in range(170,256))):
                    row[j] = (0,0,0,255)
                    continue
                if(color[0] in range(170,195)):line=MISSLINE
                else:line = MISSLINE
                if(color[1] in range(60,140)):
                    dis = line.CalcDistance(Point(color[1],color[2]))
            if(dis < Limit):row[j]=(255,255,255,255) 
            else:row[j] = (0,0,0,255)
    
    Image.fromarray(arr).save(path2)


