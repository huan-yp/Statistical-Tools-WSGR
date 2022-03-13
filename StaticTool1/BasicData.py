from Data.Loader import LoadPointPosition as LP
from airtest.core.cv import Template
from Req import *

BB = "BB";BC = "BC";CL = "CL";CV = "CV";CA = "CA";CVL = "CVL"
AP = "AP";DD = "DD";SS = "SS";CLT = "CLT";BM = "BM";AV = "AV";
AADG = "AADG";ASDG = "ASDG";BG = "BG";CBG = "CBG";NO = "NO"
#CBG 大巡,BG 导战
XY = (CL,CA,BC,CLT)
LS = (CL,CVL)
VS = (AV,CV,CVL)

PointPosition = {}
nodeCount=[0,5,6,4,4,5,4,5,5,5,0,0]
timeADay = 86000
BloodColor = [[[117,162,69],[51,184,246],[230,89,18],[92,91,96],[112,87,43]],[[88,182,70],[64,186,238],[3,3,166]]]
#[0]0绿色,1黄色,2红色,3黑色,4蓝色,RGB格式 
#[1]0绿色,1黄色,2红色
ExerciseColor = [[226,33,132],[247,243,244]]
BloodPos = [[[],[56,322],[168,322],[280,322],[392,322],[504,322],[616,322]],\
    [[],[60,142],[60,217],[60,292],[60,367],[60,442],[60,517]]]#0为准备界面,1为出征过程 
NumberImage = ["",]#节点数字图片
ChapterImage = ["",]#章节图片
FightImage = ["",]#战斗相关图片
ConfirmImage = ["",]#确认按钮图片
CancelImage = ["",]#取消按钮的图片
BackImage = [""]#回退按钮的图片
TURN = [(0,0),(1,0),(-1,0)]
EnemyPos = [[(277,312,309,328),(380,312,412,328),(483,312,515,328),(587,312,619,328),(690,312,722,328),(793,312,825,328)],\
            [(39,156,71,172),(322,156,354,172),(39,245,71,261),(322,245,354,261),(39,334,71,350),(322,334,354,350)]]
            #敌人位置 [0] 为演习,[1] 为出征
ConditionPos = [None,(207,221),(443,287),(752,196),(191,413),(733,400)]
#
#常见:DD SS AP CL CA CLT CVL BB BC CV
#不常见:AADG ASDG SC BBG BBV BG AV CAV BM
#当前路径 .\\Source\\Python
def LoadPointPosition():
    LP(PointPosition)
def LoadImage():
    print("正在加载图片路径...")
    PrePath = os.getcwd() + "\\Data\\"

    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\1.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\2.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\3.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\4.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\5.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\6.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\7.PNG",resolution=(960,540)))
    ChapterImage.append(Template(PrePath + "Image\\ChapterImage\\8.PNG",resolution=(960,540)))
    
    """NumberImage.append(Template(PrePath + "Image\\number\\1.PNG",resolution=(960,540)))
    NumberImage.append(Template(PrePath + "Image\\number\\2.PNG",resolution=(960,540)))
    NumberImage.append(Template(PrePath + "Image\\number\\3.PNG",resolution=(960,540)))
    NumberImage.append(Template(PrePath + "Image\\number\\4.PNG",resolution=(960,540)))
    NumberImage.append(Template(PrePath + "Image\\number\\5.PNG",resolution=(960,540)))
    NumberImage.append(Template(PrePath + "Image\\number\\6.PNG",resolution=(960,540)))"""


    FightImage.append(Template("Data\\Image\\FightImage\\1.PNG",resolution=(960,540)))#选择阵型
    FightImage.append(Template("Data\\Image\\FightImage\\2.PNG",resolution=(960,540)))#开始战斗
    FightImage.append(Template("Data\\Image\\FightImage\\3.PNG",resolution=(960,540)))#点击继续
    FightImage.append(Template("Data\\Image\\FightImage\\4.PNG",resolution=(960,540)))#大破回港
    FightImage.append(Template("Data\\Image\\FightImage\\5.PNG",resolution=(960,540)))#前进回港
    FightImage.append(Template("Data\\Image\\FightImage\\6.PNG",resolution=(960,540)))#追击回港
    FightImage.append(Template("Data\\Image\\FightImage\\7.PNG",resolution=(960,540)))#舰船图标1
    FightImage.append(Template("Data\\Image\\FightImage\\8.PNG",resolution=(960,540)))#舰船图标2
    FightImage.append(Template("Data\\Image\\FightImage\\9.PNG",resolution=(960,540)))#困难模式
    FightImage.append(Template("Data\\Image\\FightImage\\10.PNG",resolution=(960,540)))#选择战况
    FightImage.append(Template("Data\\Image\\FightImage\\11.PNG",resolution=(960,540)))#自动补给标志
    FightImage.append(Template("Data\\Image\\FightImage\\12.PNG",resolution=(960,540)))#自动补给标志


#加载图片



























