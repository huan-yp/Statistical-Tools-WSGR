from math import sqrt
import math
class Point():
    def __init__(self,x=None,y=None,point=None):
        if(point != None):self.x,self.y = point
        else:self.x,self.y = x,y
class Line():
    #二维直线
    def __init__(self,Point1 = None,Point2 = None,k=None,b=None):
        if(Point1 != None and Point2 !=None):
            self.k=(Point1[1]-Point2[1])/(Point1[0]-Point2[0])
            self.b=Point2[1]-self.k*Point2[0];
        else:
            self.k=k
            self.b=b
        self.k=float(self.k)
        self.b=float(self.b)
    def CalcIntersection(self,line):
        #计算与另一条直线的交点
        if(type(self)!=type(line)):
            raise ValueError("Argument is not a line")
        x = (self.b-line.b)/(-self.k+line.k)
        y = self.k*x+self.b
        return (x,y)
    def CalcDistance(self,point:Point):
        return abs(self.k*point.x-point.y+self.b)/sqrt(1+self.k**2)
def DistanceBetweenLineAndPoint(line:Line,point:Point):
    #传入的 line 为 Line 类,Point 为一个二元组
    return line.CalcDistance(point)
def LineIntersection(line1:Line,line2:Line):
    return line1.CalcIntersection(line2)
def GetLine(Point1:Point,Point2:Point):
    return Line(tuple(Point1),tuple(Point2))
def CalcED(point1,point2,HeightList = []) -> float:
    if(len(point1) != len(point2)):
        raise ValueError("Can't Calculate Distance Between Two Points With Different Dimensions")
    HeightList = [1]*len(point2)
    dis,cnt = 0,0
    for x,y,h in zip(point1,point2,HeightList):
        dis += (x-y)**2*h
    return sqrt(dis)

