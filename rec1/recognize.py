from ast import Num
from tokenize import Number
from PIL import Image
from numpy import array
import easyocr
import os
NumberImage = []
resulotion = ()
#size:250,125
Box = [0,(1198,565),(1385,500),(1176,378),(1382,311),(1075,230),(1235,157)]
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
    ClearFile(path)
    file = open(path,"a+")
    file.write(str(img.size[1])+" "+str(img.size[0])+"\n")
    arr = array(img)
    for x in arr:
        for y in x:
            file.write(str(y)+" ")
        file.write("\n")
    file.close()
def Triple(element):
    for i,x in enumerate(element):
        element[i] = [x] * 3 
def ReadImage(path):
    #图片的数据格式:
    #从 path 读取数据格式并转换为图像返回
    #格式为第一行两个数 n,m
    #接下来 n 行，每行 m 个数，有空格分开，表示灰度值
    file = open(path,"r")
    lis = []
    n,m=list(map(int,file.readline().split()))
    for i in range(n):
        x = list(map(int,file.readline().split()))
        Triple(x)
        lis.append(x)
    file.close()
    arr = array(lis)
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
    WriteGeryImage(ig,"data.in")
    os.system("Split")
    res = 0
    file = open("data.out","r")
    cnt = int(file.readline())
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
        ToBlackWhite(path1,path2,box=(Box[j][0],Box[j][1],Box[j][0]+186,Box[j][1]+166))
        res.append(RecognizeNumber(path2))
    return res
def CheckMiss1():
    pass
def CheckMiss2():
    pass
def GetPosImage(path1,path2,pos):
    #将 path1 的 pos 部分导出到 path2
    img = Image.open(path1).convert("L")
    img.crop((Box[pos][0],Box[pos][1],Box[pos][0]+180,Box[pos][1]+165)).save(path2)


"""print(Recognize("1.png"))"""
"""ToBlackWhite("1.PNG","TMP1.PNG",box=(Box[2][0],Box[2][1],Box[2][0]+180,Box[2][1]+135))
print(RecognizeNumber("TMP1.PNG"))"""
ClearFile("res.out")
file = open("res.out","a")
"""GetPosImage("1.PNG","11.PNG",4)
WriteGeryImage(Image.open("11.PNG"),"data.in")"""
Reader = easyocr.Reader(["en"])
path = "1.png"
for i in range(1,7):
    path = str(i)+".PNG"
    #img = open(path)
    #resulotion = (img.size[0],img.size[1])
    #print(Recognize(path))
    Image.open(path).convert("L").resize((1600,900)).save(path)
    file.write(str(i)+":"+str(Recognize(path))+"\n")
    for j in range(1,7):
        os.system("del "+ str(j) + "T" +path)
        pass
file.close()


