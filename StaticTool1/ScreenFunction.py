from airtest.core.cv import locateCenterOnScreen
from airtest.core.cv import locateAllOnScreen
from Req import *
from Req import *
#处理获取的信息
def ImageCount(query,NeedScreenShot = 1,confidence = 0.9):
    result = GetAllImage(query,NeedScreenShot,confidence)   
    return len(result)
def GetAllImage(img,NeedScreenShot = 1,confidence=0.9):
    if(NeedScreenShot == 1):
        UpdateScreen()
    return locateAllOnScreen(img,screen=RD.screen,confidence=confidence)
def GetImagePosition(img,NeedScreenShot = 1,confidence=0.9,MyMethods = ["tpl"]):
    #存在返回位置(tuple)，否则返回 None
    #传入 airtest Template 类型
    #存在位置为 (960*540) 标准
    if(NeedScreenShot == 1):
        UpdateScreen()
    res = locateCenterOnScreen(img,screen=RD.screen,confidence=confidence,MyMethods=MyMethods) 
    if(res == None):
        return None
    r = RD.resolution[0]/960
    ret = (int(res[0]/r),int(res[1]/r))
    return ret
def ImageExist(img,NeedScreenShot = 1,confidence = 0.9,MyMethods = ["tpl"]):
    result = GetImagePosition(img,NeedScreenShot,confidence,MyMethods)
    if(result == None):
        return False
    return True
def WaitImage(img,confidence  = 0.9,timeout = 10,MyMethods = ["tpl"]):
    #成功返回位置(tuple)，否则返回 False
    StartTime = time.time()
    while(True):
        x = GetImagePosition(img,1,confidence,MyMethods)
        if(x != None):
            return x
        if(time.time()-StartTime > timeout):
            time.sleep(0.15)
            return False
        if(time.time()-StartTime > 200):
            raise ValueError
def WaitImages(ImageList = [],ConfidenceList=[0.9,0.9]*10,TimeOut = 10):
    if(len(ImageList) > len(ConfidenceList)):
        print("Error In WaitImages:length do not equal")
        raise ValueError
    StartTime = time.time()
    while(True):
        UpdateScreen()
        for i in range(0,len(ImageList)):
            if(ImageList[i] == None):
                continue
            if(ImageExist(ImageList[i],0,ConfidenceList[i])):
                return i
        if(time.time()-StartTime > TimeOut):
            return None






