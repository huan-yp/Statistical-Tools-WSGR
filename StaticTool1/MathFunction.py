from Req import *
def CalcDis(p1,p2):
    if(len(p1) != len(p2)):
        print("Error:Dimensions do not equal")
        raise TypeError
    square = 0
    for i in range(0,len(p1)):
        square += (p1[i]-p2[i])*(p1[i]-p2[i])
    return square
def CheckColor(col,ColorList):
    #传入一个颜色 BGR 和一个可能颜色列表 BGR
    #列表下标从 0 开始，返回和第几个颜色最相近
    result = 0
    for i in range(1,len(ColorList)):
        if(CalcDis(col,ColorList[i]) < CalcDis(col,ColorList[result])):
            result = i
    return result
def CalcShipPoint(c,n):
    pos = RD.ShipPosition
    dir = PointPosition
    ret = "A"
    for i in range(0,26):
        ch = chr(ord('A')+i)
        node1 = (c,n,ch);node2 = (c,n,ret)
        if((node1 in dir) == False):
            break
        if(CalcDis(dir[node1],pos) < CalcDis(dir[node2],pos)):
            ret = ch
    return ret