from Req import *
from BasicData import *
from ScreenFunction import *
from WriteData import ClearArgs,ClearFile,WriteArgs,WriteMatrixAsArgs
def GetEnemyCondition(type):
    #0演习
    StartTime = time.time()
    result = {CV:0,BB:0,SS:0,BC:0,AP:0,DD:0,ASDG:0,AADG:0,CL:0,CA:0,CLT:0,CVL:0,NO:0,AV:0,\
    BM:0,}
    all = 0
#    starttime = time.time()
    #snapshot("screen.PNG")
    UpdateScreen()
    PIM.fromarray(RD.screen).convert("L").save(RD.WORKPATH+"\\Data\\Tmp\screen.PNG")
    img = PIM.open(RD.WORKPATH+"\\Data\\Tmp\\screen.PNG")
#    RD.TimeCount+= time.time()-starttime
    ClearArgs()
    ClearFile(RD.WORKPATH + "\\Data\\Tunnel\\res.out")
    WriteArgs("recognize\n",RD.TUNNELPATH + "\\Args.in")
    WriteArgs("6\n",RD.TUNNELPATH + "\\Args.in")
    cnt = 0 
    for tmp in EnemyPos[type]:
        cnt += 1
        r = RD.resolution[0]/960
        x = (int(tmp[0]*r),int(tmp[1]*r),int(tmp[2]*r),int(tmp[3]*r))    
        arr = array(img.crop(x))
        WriteMatrixAsArgs(arr)
    #Remeber To Close The Fucking Reader
    inidir = os.getcwd()
    os.chdir(RD.TUNNELPATH)
    os.system(".\Source.exe")
    os.chdir(inidir)
    res = ReadFile(RD.TUNNELPATH + "\\res.out")
    result["ALL"] = 0
    for x in res:
        result[x] += 1
        if(x != NO):
            result["ALL"] += 1
    
    print("TimeCostGetEnemy:",time.time()-StartTime)
    print(result)
    return result
def UpdateShipPosition():
    pos = GetImagePosition(FightImage[7],0,0.8) 
    if(pos == None):
        pos = GetImagePosition(FightImage[8],0,0.8)
    if(pos == None):
        return
    RD.ShipPosition = pos



