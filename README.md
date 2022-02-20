# Statistical-Tools-WSGR

## 简介

面向工作室的战舰少女统计工具，正在开发中

## 现有功能

### 图片辅助记录

运行 DataTools.main ，初始化完成后按下 `p` 截取模拟器屏幕并按顺序保存，帧率不会低于你按键盘的速度，使用后及时保存，下一次会被覆盖。

### 简单视频数据转换

传入一个视频地址和默认战斗间隔，返回一个字符串表示统计数据，请自行匹配解码。

要求攻击全部为单点攻击。

**跳弹会被认成未命中，这个问题下个版本修复。**

**反潜的位置判断相对困难，下个版本做。**

**伤害判定是用 `Python` 写的，下个版本合并**

默认战斗间隔表示如果超过这个时间仍然未发现敌方被攻击，那么认为这段时间开始了下一场战斗。

视频 `FPS>=6`，分辨率 `16:9`,大小不超过 `1600x900`，可以有轻微误差。

**算法模型是根据原画制作的，所以请保留原画视频传入，不要做任何压缩，否则不保证准确率。**

电脑可以使用 `EV录屏` 录制模拟器画面，清晰度选择原画。

**请预留视频占据空间两倍以上加 400MB 的运行内存，如果视频大小为 1600MB,请预留3.6GB 的运行内存，不建议传入太长的视频，主要原因是如果出 BUG 了要反应很久。 **

#### 样例传入

```
C:\Users\Administrator\Desktop\Test.mp4:str,
20:int
```

#### 样例传出

```
"Fight1:


Fight2:
Position:4,Type:NORMAL
Position:2,Type:MISS
Position:2,Type:YELLOW
Position:1,Type:NORMAL
Position:6,Type:NORMAL
Position:5,Type:NORMAL
Position:3,Type:NORMAL


Fight3:
Position:1,Type:NORMAL
Position:4,Type:NORMAL
Position:4,Type:NORMAL
Position:3,Type:NORMAL
Position:6,Type:NORMAL
Position:2,Type:NORMAL
Position:2,Type:MISS
Position:2,Type:NORMAL
Position:5,Type:NORMAL
Position:3,Type:NORMAL"


```

#### 样例视频：

见 `\TestData\TestVideo\Test.mp4`

这是用 `EV` 录屏录制的，建议按这个分辨率和清晰度来。

## 依赖和使用建议

`C++` 文件已经配置好了，请将整个 `API` 文件夹拷贝下来使用，如果仍然无法使用，可以安装 `opencv4.5.1` 版本并配置好相关环境，也可以直接联系作者 `QQ:3051561876` 。

`Python` 相关依赖在 `Requirements.txt` 中写有，建议使用虚拟环境安装，`Python` 建议版本为 `Python3.8.0`。

## 文档：

见 `\Docu`

## 额外功能：
如果需要更完善的功能麻烦发 $\text{Issue}$.
