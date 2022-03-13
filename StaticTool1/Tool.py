from PreSet import *
from GetGameInfo import *
from MathFunction import *
from airtest.core.api import snapshot
from tkinter import messagebox
def ConsoleInfo(info,FileName = "log.txt"):
    with open(FileName,mode="a+") as f:
        f.write(info)
def SingleRound(c,n):
    print("Waiting Start")
    WaitImage(ChapterImage[c],0.9,120)
    print("SingleRoundStart,Recording")
    Get = 0 
    RD.ShipPoint = "Z"
    while(True):
        RD.FightCount += 1
        StartTime = time.time()
        for i in range(1,100000):
            if(ImageExist(ChapterImage[c]) and Get == 1):
                print("RoundFinish")
                return
            UpdateScreen()
            UpdateShipPosition() 
            if(ImageExist(FightImage[2],0)):
                    if(CalcShipPoint(c,n) == RD.ShipPoint):
                        continue
                    print("EnemyCathced,Recognizing")
                    RD.ShipPoint = CalcShipPoint(c,n)
                    print("FigthPoint:",RD.ShipPoint)
                    print("Condition:",ImageExist(FightImage[12]))
                    ConsoleInfo(str(RD.ShipPoint)+":","log2.txt")
                    ConsoleInfo(str(GetEnemyCondition(1))+"\n","log2.txt")
                    ConsoleInfo(str(RD.ShipPoint)+":")
                    ConsoleInfo("Condition:"+str(ImageExist(FightImage[12],confidence=0.85))+"\n")
                    Get = 1
                    break
        print("EnemyConditionRecorded,WaitingNextFight")   
        

def Count(c = 2,n = 5):
    print("正在测试连接情况")
    WaitImage(ChapterImage[1],timeout=1)#强制等待
    print("测试结束，请输入目标地图，一行两个正整数，示例：")
    print("2 5")
    print("请注意，开始后上一次的统计数据将会被删除，请及时保存")
    c,n = map(int,input().split())
    os.system("del log.txt")
    os.system("del log2.txt")
    print("正在载入相关数据，")
    for i in range(500):
        try:
            SingleRound(c,n)
        except Exception as e:
            print("error:",e)
            messagebox.showerror("警告","程序崩溃，请手动停止程序并记录当前数据，然后重启程序")
            return
snapshot("1.PNG")
Count()

