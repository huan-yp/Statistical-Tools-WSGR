# -*- encoding=utf8 -*-
from airtest.core.api import auto_setup,snapshot
import time,os
from keyboard import hook
WORKPATH = r"C:\Users\Administrator\Desktop\Counts"
__author__ = "Phantom_Peng"
times = 0
def Init(Device):
    auto_setup(WORKPATH,devices=["android://127.0.0.1:5037//"+Device+"?cap_method=MINICAP&&ori_method=MINICAPORI&&touch_method=MINITOUCH"])
def Rec(event):
    global times
    if(event.name !='p' or event.event_type == 'down'):
        return
    print("recorded")
    times += 1
    snapshot(str(times)+".PNG")
def GetAndroidInfo():
    info = os.popen("adb devices -l")
    time.sleep(5)
    info = os.popen("adb devices -l")
    res = [];get = 0
    for x in info:
        if(get == 0):
            get = 1;continue
        if(len(x.split()) == 0):
            break;
        a = x.split()[0]
        res.append(a)
    print("Devices list:",res)
    return res
def Count():
    time.sleep(10000)
print("开始初始化")
os.chdir(WORKPATH)
os.system("adb devices -l")
os.system("adb device -l")
try:
    Device = GetAndroidInfo()[0]
except:
    print("Error,No Device Found")
Init(Device)
snapshot("0.PNG")
print("初始化完成")
hook(Rec)
Count()
def NetWorkChecker():
    for i in range(1,10000):
        time.sleep(60)


