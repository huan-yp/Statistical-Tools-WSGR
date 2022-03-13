from Req import *
from airtest.core.api import auto_setup
#Win 和 Android 的通信
#Win 向系统写入数据
def GetAndroidInfo():
    info = os.popen(RD.WORKPATH + "\\adb.exe devices -l")
    time.sleep(5)
    info = os.popen(RD.WORKPATH + "\\adb.exe devices -l")
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
def CopyRequirements(path):
    try:
        os.system("rm "+path+"\\airtest")
    except Exception as e:
        print(e)
    print(path)
    try:
        shutil.rmtree(path + "\\airtest")
    except:
        pass
    print("xcopy req "+ path + " /E/H/C/I")
    os.popen("xcopy req "+ path + " /E/H/C/I")
    time.sleep(5)
def ConnectAndroid(TryTimes = 0):
    devices = GetAndroidInfo()
    print("正在准备图像依赖，请稍后")
    CopyRequirements(os.path.dirname(__file__))
    if(TryTimes > 3):
        print("ADB Crashed,Checking")
        RestartAndroid(TryTimes - 1)
        ConnectAndroid(0)
        return
    try:
        print("正在连接模拟器")
        auto_setup(__file__,devices=["android://127.0.0.1:5037//"+devices[0]+"?cap_method=MINICAP&&ori_method=MINICAPORI&&touch_method=MINITOUCH",])
        print("Hello")
    except Exception as e:
        print("Error:",e)
        print("Checking ADB connection")
        try:
            os.system(RD.WORKPATH + "\\adb.exe devices -l")
            os.system(RD.WORKPATH + "\\adb.exe devices -l")
        except:
            print("Failed,ADB Crashed")
        else:
            print("ADB Fixed,Trying Start up")
            ConnectAndroid(TryTimes + 1)
def RestartAndroid(times):
    return 
    print("Android Restaring")
    try:
        dir = os.getcwd()
        try:
            os.system("taskkill -f -im dnplayer.exe")
        except:
            pass
        os.chdir(S.RESTARTPATH)
        os.popen(r".\dnplayer.exe")
        os.chdir(dir)
    except:
        pass
    print("Waiting")
    time.sleep(15 * (times - 1))
def CheckNetWork():
    #检查网络，正常返回 Ture
    return os.system("ping baidu.com") == False
def Print(Debug = 1 ,*args):
    if(Debug == 0):
        pass
    print(args)
def LogImage(Img,Name):
    Path = ".\\Data\\Log\\" + Name
    try:
        Img = PIM.fromarray(Img)
    except:    
        pass;
    Img.save(Path)





