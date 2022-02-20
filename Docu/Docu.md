# Statistical-Tools-WSGR

## PythonAPI:

### Exporter.py:

功能：把 `C++` 写的功能全部导出.

#### VideoRecognizer

导出主要功能

```python
def RecognizeVideo(VideoPath:str,FightGap:float):
    InitialDir = os.getcwd()
    os.chdir(RequirementPath)
    os.system("del data.in")
    with open(RequirementPath+"\\data.in",mode="a+") as file:
        file.write(VideoPath+"\n")
        file.write(str(FightGap)+"\n")
    try:
        os.system(RecognizerPath)
    except:
        print("Unexception C++ Error!")
    with open(RequirementPath+"\\data.out",mode="r") as file:
        res = file.read()
    os.chdir(InitialDir)
    return res
```

$\text{DEMO}$

```python
from API.Exporter import *
print(RecognizeVideo(r"C:\Users\Administrator\Desktop\Counts\TestData\TestVideo\Test.mp4",15))
```

#### CheckPath

检查路径依赖是否正确

```python
print("os.getcwd():",os.getcwd())
print("__file__:",__file__)
print("RequirementPath:",RequirementPath)
print("RecognizerPath:",RecognizerPath)
print("sys.path:",sys.path)
```



