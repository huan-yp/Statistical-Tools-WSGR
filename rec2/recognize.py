from PIL import Image,ImageFilter
from numpy import array
import easyocr
import numpy as np
import os
import time
from ConstVariables import *
from ImageTurn import GetKeyPoint
NumberImage = []
resulotion = ()
FileTime = 0
import cv2

"""暴击检测思路
采用渐变色二值化法
观察发现颜色处于两种相差较大的黄色的渐变色
两种颜色分别为 (253,237,90),(249,185,35)
计算每个像素点和两个点的欧几里得距离的最小值。
不超过阈值 20 即认为是可行点
按照点的可行与否进行二值化后降噪
如果降噪失败则再次计算每个像素点到原点的距离，取阈值 20 左右作为判定黑点的依据
进行边框处理再次降噪
降噪后期望图中只存在 '击' '穿'
提取出每个字，检验同一排是否存在对应字 
计算期望中心和实际中心欧几里得距离，设定阈值 5 作为依据。
"""
"""未命中，跳弹检测思路
同样采用渐变色法二值化
注意判定二者的不同，尽量避免同时出现，如果出现采用追踪算法
"""
"""图像直线追踪
PRE:
提取视频中三帧图像
按追踪模板的特性对视频进行二值化并降噪
截取出期望位置
STEP1:提取图像特征点
STEP2:对期望位置尝试寻找目标并匹配
"""


os.chdir(os.getcwd()+"\\rec2")

def ClearFile(path):
    os.system("del "+path)
def CheckBoom():
    pass
def ToGrey(path):
    #将 Path 图像转换为灰度图，并覆盖
    Image.open(path).convert("L").save(path)
def WriteGeryImage(img,path):
    #PIL Image 格式传入灰度图 img
    #向 path 中以数据格式写入
    global FileTime
    time1=time.time()
    ClearFile(path)
    file = open(path,"a+")
    file.write(str(img.size[1])+" "+str(img.size[0])+"\n")
    arr = array(img)
    for x in arr:
        for y in x:
            file.write(str(y)+" ")
        file.write("\n")
    file.close()
    FileTime+=time.time()-time1
def Triple(element):
    for i,x in enumerate(element):
        element[i] = [x] * 3 
def ReadImage(path):
    #图片的数据格式:
    #从 path 读取数据格式并转换为图像返回
    #格式为第一行两个数 n,m
    #接下来 n 行，每行 m 个数，有空格分开，表示灰度值
    global FileTime
    time1=time.time()
    file = open(path,"r")
    lis = []
    n,m=list(map(int,file.readline().split()))
    for i in range(n):
        x = list(map(int,file.readline().split()))
        Triple(x)
        lis.append(x)
    file.close()
    arr = array(lis)
    FileTime += time.time()-time1
    return Image.fromarray(arr.astype("uint8"))
def ToBlackWhite(path1,path2,box = None):
    #传入源路径和目标路径
    #将原图像处理后传入 path2s
    ClearFile("data.in")
    ToGrey(path1)
    img = Image.open(path1)
    if(box != None):
        img = img.crop(box)
    WriteGeryImage(img,"data.in")
    os.system("Binarization.exe")
    img = ReadImage("data.out")
    img.save(path2)
def GetNumberFromList(list):
    #从字符列表中获取数字
    res = 0
    for x in list:
        res = res * 10 + x
    return res
def RecognizeNumber(path):
    #处理：c++实现的二值化，降噪，滤波，边框提取
    #传入已经处理过的图片路径，返回数字
    #只能包含一个伤害
    ig = Image.open(path).convert("L")
    ig.save(path)

    WriteGeryImage(ig,"data.in")
    os.system("Split")
    res = 0
    file = open("data.out","r")
    cnt = int(file.readline())
    if(cnt == 0):
        return 0

    rec = Reader.readtext(path)
    print(rec)
    if(len(rec) == 0):
        return 0

    text = rec[0][1]
    NumberList = []
    CheckAgain = 0

    for st in text:
        if(st == '|'):
            NumberList.append(1)
            continue
        if(IsIntenger(st) == False):continue
        NumberList.append(int(st))
        if(int(st) == 1):
            CheckAgain = 1
    if(CheckAgain == 0):
        return GetNumberFromList(NumberList)
    for i in range(len(NumberList)):
        box = tuple(map(int,file.readline().split()))
        """if(box[2] - box[0] <= 15):
            NumberList[i] = 1"""
        if(NumberList[i] == 1 and box[2] - box[0] >= 18):
            NumberList[i] = 7
        NumberList[i] = int(NumberList[i])


    return GetNumberFromList(NumberList)
    """
        path2 = "TMP2.PNG"
        img = ig.crop(box)
        img.save(path2)
        nw,nh = (84,84)
        nimg = Image.new('RGB',(nw,nh),(0,0,0))
        img = img.resize((img.size[0]*2,img.size[1]*2))
        nimg.paste(img,((nw-img.size[0]) // 2, (nh-img.size[1]) // 2))
        nimg.save(path2)
        text = Reader.readtext(path2)
        for x in text:
            if(len(x) > 0):
                if(x[2] < 0.75):
                    continue
                print(x)
                x = x[1]
                try:
                    int(x)
                except:
                    pass
                else:
                    res = res * 10 + int(x)
    file.close()"""
def IsIntenger(x):
    #判断字符串是否为整数
    try:
        int(x)
    except:
        return False
    else:
        return True
def Recognize(path1):
    #识别 path1，判断伤害值
    
    res = [0]
    for j in range(1,7):
        path2 = str(j)+"T"+path1
        ToBlackWhite(path1,path2,box=(BOX[j][0],BOX[j][1],BOX[j][0]+186,BOX[j][1]+166))
        res.append(RecognizeNumber(path2))
    return res
def CheckMiss1(path1):
    res = [0,]
    for j in range(1,7):
        path2 = str(j)+"T"+path1
        path3 = str(j)+"TT"+path1
        GetPosImage(path1,path2,j)
        GetKeyPoint(path2,path2,MISSLINE,Limit=12)
        WriteGeryImage(Image.open(path2).convert("L"),"data.in")
        #os.system("del "+path2)
        os.system("CountMiss")
        ReadImage("data.out").save(path3)
        with open("res.out",mode="r") as file:
            cnt = int(file.readline())
            res.append(max((cnt-3)/2,0))
            print(cnt)
    print(res)
    return res
def CheckMiss2():
    pass
def GetPosImage(path1,path2,pos):
    #将 path1 的 pos 部分导出到 path2
    img = Image.open(path1)
    img.crop((BOX[pos][0],BOX[pos][1],BOX[pos][0]+180,BOX[pos][1]+165)).save(path2)
def Filtering(path):
    img = Image.open(path)
    img = img.filter(ImageFilter.DETAIL)
    img.save(path)
cap = cv2.VideoCapture("Test.MP4")
cap.release()
print("OK")
"""Reader = easyocr.Reader(["en"])
CheckMiss1("1.PNG")"""


