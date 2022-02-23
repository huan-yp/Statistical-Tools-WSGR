

/*
Creted:2022.2.16
Author:Huan_yp/Phantom_Peng

Function:Switch video to data in WSGR
Requirements:opencv4.5.1,MSVC,Win10
需要实现的功能：
	从工作目录下 data.in 中读取需要处理的图片路径列表
	输出结果到 data.out
	结果的形式为每张图片一个六元组
	具体功能：
	读取一张 16:9 完整游戏图片并进行未命中，跳弹，暴击，命中的判定

	实现思路：
	1.降噪：
	对于跳弹和未命中和暴击:
	(1)对目标颜色区域建立回归直线方程，取固定阈值为是否是关键点的第一个判段依据
	(2)建立边框色彩特征直线方程，取基于分辨率的动态阈值作为判断是否为边框的依据
	(3)按照边框对像素点进行划分，以是否在边框内作为是否为关键点的第二个判断依据
	(4)按照是否为关键点二值化，将每个关键点块矩阵化并消除椒盐噪点
	 2.识别：
	(1)简单情景识别：判断是否存在矩阵块
	(2)复杂情景识别：


	<1>将关键部分作为矩阵提取出来，按照位置排序
	<2>描绘出每个关键部分的轮廓，提取特征点进行匹配
	特征点选取和匹配怎么做还没想好
*/

#include<Operation.h>
#include<Debug.h>
#include<RecognizeNumber.h>
using namespace std;

#define INF 1000000000
using namespace std;

int DEBUG = 0;
int TESTTIME = 0;
int CONSOLE = 1;
int CONSOLERES = 1;

const int BIRGHT = 18;
const int BOX[] = { {},{},{},{},{},{},{} };\
const int BLUE1[] = { 15,121,174 };
const int BLUE2[] = { 64,212,223 };

int flag = 1, have, skip, FrameNow;
int StatuCount[12];

double VideoGap, VideoRate;

string FightStatuNow;
MathMatri MathMat[12];
ofstream ot("res.out");
string NumberToString(int num) {
	if (num == 0)return "0";
	string res = "";
	res += char(num % 10 + '0');
	num /= 10;
	if (num == 0)return res;
	return NumberToString(num) + res;
}

namespace Split {
	//提取有效部分
}
namespace VideoFunction {
	void SkipFrame(double second) {
		FrameNow += second * 1000.0 / VideoGap;
	}
}
namespace NoiseReduction {
	//针对不同情景下的降噪
	const int N = 1605, M = 705;
	const int dx[] = { 1,1,0,0 }, dy[] = { 1,0,1,0 };
	int Border = 20, n, m, cnt = 0, s = 0;
	int rk[N][M], fa[N * M], size[N * M], KeyPointCount[10];
	int maxn[N * M][2], minu[N * M][2], vis[N][M], KPC = 0;
	bool border[N][M], in[N][M],tvis[N][M];
	int line[N];
	queue<pair<int, int>> q;
	//vis[i][j]:-1->未计算,0:在边框内,1:不在边框内
	//淡化图像边界提取效果很差，边界提取算法需改进
	bool ok(int x, int y) {
		return x >= 0 && y >= 0 && x < n&& y < m;
	}

	void bfs() {
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (border[i][j])
					vis[i][j] = 0;
		for (int i = 0;i < n;i++) {
			if (vis[i][0] == -1)q.push(make_pair(i, 0)), vis[i][0] = 1;
			if (vis[i][m - 1] == -1)q.push(make_pair(i, m - 1)), vis[i][m - 1] = 1;
		}
		for (int i = 0;i < m;i++) {
			if (vis[n - 1][i] == -1)q.push(make_pair(n - 1, i)), vis[n - 1][i] = 1;
			if (vis[0][i] == -1)q.push(make_pair(0, i)), vis[0][i] = 1;
		}
		const int dxx[] = { 1,-1,0,0, }, dyy[] = { 0,0,1,-1 };
		while (!q.empty()) {
			int x = q.front().first, y = q.front().second;
			q.pop();
			for (int k = 0;k < 4;k++) {
				int tox = x + dxx[k], toy = y + dyy[k];
				if (vis[tox][toy] != -1 || !ok(tox, toy))continue;
				vis[tox][toy] = 1;
				q.push(make_pair(tox, toy));
			}
		}
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (vis[i][j] == -1)
					vis[i][j] = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				if (border[i][j])
					vis[i][j] = 1;
				tvis[i][j] = vis[i][j];
			}
	}
	void SetBorder() {
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				vis[i][j] = tvis[i][j];
	}
	int find(int x) {
		return fa[x] == x ? x : fa[x] = find(fa[x]);
	}
	void merge(int u, int v) {
		u = find(u), v = find(v);
		if (u == v)return;
		fa[u] = v;size[v] += size[u];
		cmax(maxn[v][0], maxn[u][0]), cmax(maxn[v][1], maxn[u][1]);
		cmin(minu[v][0], minu[u][0]), cmin(minu[v][1], minu[u][1]);
	}
	void ShowBorder() {
		ImageMatri img;
		img.n = n, img.m = m;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				img.g[i][j] = 255 * border[i][j];
		//Border 为白色
		if (DEBUG)img.SaveGrey("Border.PNG");
	}
	bool CheckBorder(const short* a, int limit) {

		double r = a[0], g = a[1], b = a[2];
		if ((abs(r - g) <= limit && abs(g - b) <= limit && abs(r - b) <= limit))
			return 1;
		return 0;
	}
	bool CheckWhite(const short* a) {
		double r = a[0], g = a[1], b = a[2];
		if ((abs(r - g) <= 30 && abs(g - b) <= 30 && abs(r - b) <= 30))
			return 1;
		return 0;
	}
	void ShowFill(ImageMatri Target) {
		int count = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				Target.g[i][j] = vis[i][j] * 255;
				if (Target.g[i][j] == 0)count++;
			}
		//黑色为有效
		if (DEBUG)Target.SaveGrey("Fill.PNG");
		if (count == 0 || count > n * m / 3)
			skip = 1;
	}
	void GetKeyPoint(ImageMatri& Target, string Type, double DisLimit = 18) {
		//传入一个图形矩阵，分析特征点并写入灰度值
		KPC = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				
				if (vis[i][j]) {
					Target.g[i][j] = 0;
					continue;
				}
				if (Type == "MISS") {
					double h, s, v;
					Target.g[i][j] = 0;
					rgb_to_hsv(Target.a[i][j][0], Target.a[i][j][1], Target.a[i][j][2], h, s, v);
					if (((h <= 0.1 || h >= 0.97) && s >= 0.3 && Target.a[i][j][2] <= 168 && v >= 0.45))
						Target.g[i][j] = 255, KPC++;
					else
						Target.g[i][j] = 0;
				}
				if (Type == "YELLOW") {
					Target.g[i][j] = 0;
					Point p = Point(Target.a[i][j][1], Target.a[i][j][2]);
					double h, s, v;
					rgb_to_hsv(Target.a[i][j][0], Target.a[i][j][1], Target.a[i][j][2], h, s, v);
					if ((h <= 0.21 && h >= 0.08 && s >= 0.3 && Target.a[i][j][2] <= 168 && v >= 0.6))
						Target.g[i][j] = 255, KPC++;
				}
				if (Type == "NORMAL") {
					if (Target.g[i][j] <= 168) { 
						Target.g[i][j] = 0;
						continue; 
					}
					if (CheckWhite(Target.a[i][j]))
						Target.g[i][j] = 255, KPC++;
					else
						Target.g[i][j] = 0;
				}
			}
	}
	void GetBorder(const ImageMatri& Target,double BorderBase = 35) {
		skip = 0;
		int BorderCount = 0;
		//Border = 1.0 * 35 / (1.0 * Target.resolution / 1600);
		Border = 1.0 * BorderBase * pow((1.0 * 1600 / Target.resolution),0.63);
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				int maxn = max(max(Target.a[i][j][0],Target.a[i][j][1]), Target.a[i][j][2]);
				border[i][j] = Target.g[i][j] <= Border&&maxn<=Border * 1.7;
				BorderCount += border[i][j];
			}
		for(int times = 1;times <= 3;times++)
			for (int i = 1;i < n;i++)
				for (int j = 1;j < m;j++) {
					int maxn = max(max(Target.a[i][j][0], Target.a[i][j][1]), Target.a[i][j][2]);
					if (border[i][j] == 0 && (border[i + 1][j] + border[i - 1][j] + border[i][j + 1] + border[i][j - 1]
						+ border[i + 1][j + 1] + border[i + 1][j - 1] + border[i - 1][j - 1] + border[i - 1][j + 1]) >= 2) {
						border[i][j] = Target.g[i][j] <= Border * 1.5 && CheckBorder(Target.a[i][j], Border * 3) && maxn <= Border * 1.7;
						BorderCount += border[i][j];
					}
				}
		if (DEBUG)ShowBorder();
		if (BorderCount >= n * m / 3 || BorderCount <= 100)skip = 1;
	}
	void Merge(ImageMatri& Target, string Type, int record = 0) {
		//合并关键点
		cnt = 0;flag = 1;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (vis[i][j])
					Target.g[i][j] = 0;
		if (DEBUG)Target.SaveGrey("Now.PNG");
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				for (int k = 0;k < 4;k++) {
					int tox = i + dx[k], toy = j + dy[k];
					if (Target.g[i][j] == Target.g[tox][toy] && tox < n && toy < m)
						merge(rk[i][j], rk[tox][toy]);
				}
			}
		if (Type == "MISS") {
			for (int i = 0;i < n;i++)
				for (int j = 0;j < m;j++) {
					for (int k = 0;k < 4;k++) {
						int tox = i + dx[k] * 2, toy = j + dy[k] * 2;
						int u = find(rk[i][j]), v = find(rk[tox][toy]);
						if (Target.g[i][j] == Target.g[tox][toy] && tox < n && toy < m && size[u] <= 150 && size[v] <= 150)
							merge(rk[i][j], rk[tox][toy]);
					}
				}
		}
		int cntp=0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				int u = rk[i][j];
				if (find(u) != u||Target.g[i][j]==0||size[u] < 50)continue;
				line[++cntp] = maxn[u][0]+minu[u][0]>>1;
			}
		if (Type == "NORMAL" && cntp < 2)return;
		if (Type == "MISS" && cntp < 2)return;
		if (Type == "YELLOW" && cntp < 3)return;
		for(int i=0;i<n;i++)
			for (int j = 0;j < m;j++) {
				int u = rk[i][j];
				if (find(u) != u || Target.g[i][j] == 0 || size[u] < 100)continue;
				int mincount=0,pos = maxn[u][0]+minu[u][0]>>1;
				for (int k = 1;k <= cntp;k++)
					if (abs(pos-line[k]) <= 7)mincount++;
				if (mincount < 2)fa[u] = 0;
			}
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				if (Target.g[i][j] == 0)continue;
				if (find(rk[i][j]) == 0) {
					Target.g[i][j] = 0;
					continue;
				}
				int u = find(rk[i][j]);
				int height = maxn[u][0] - minu[u][0], weight = maxn[u][1] - minu[u][1];
				if (Type == "MISS") {
					if (size[u] <= 200 || height <= 15 || weight <= 15 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (Type == "YELLOW") {
					if (size[u] <= 100 || height <= 15 || weight <= 5 || weight > 37 ||height > 37 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (Type == "NORMAL") {
					if (size[u] >= 500 || size[u] <= 100 || height <= 20 || weight <= 5 || height > 37 || weight > 37 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (Target.g[i][j]) {
					flag = 0;
					if (rk[i][j] == u)
						cnt++;
					for (int k = 1;k <= 6;k++) {
						if (MathMat[k].in(i, j))
							KeyPointCount[k]++;
					}
				}
			}
		if (record == 0)return;
		if (cnt) {
			if (Type == "MISS")StatuCount[3]++, std::cout << Type << ",Amount:" << StatuCount[3] << endl;
			if (Type == "YELLOW")StatuCount[2]++, std::cout << Type << ",Amount:" << StatuCount[2] << endl;
			if (Type == "NORMAL")StatuCount[1]++, std::cout << Type << ",Amount:" << StatuCount[1] << endl;
		}
		int maxn = 0;
		for (int k = 1;k <= 6;k++)cmax(maxn, KeyPointCount[k]);
		for (int k = 1;k <= 6;k++) {
			if (KeyPointCount[k] >= 50 && maxn == KeyPointCount[k])
				FightStatuNow += "Position:" + NumberToString(k) + ",Type:" + Type + ",";
		}
	}
	void Init(const ImageMatri& Target,double BorderBase=35) {
		n = Target.n, m = Target.m;
		s = 0;flag = 1;
		GetBorder(Target,BorderBase);
		//初始化
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				vis[i][j] = -1;
		//提取边框
		bfs();
	}
	void ProcessFill(const ImageMatri &Target,string Type) {
		int s = 0;
		for (int i = 0;i <n;i++)
			for (int j = 0;j < m;j++) {
				rk[i][j] = ++s;
				fa[s] = s, size[s] = Target.g[i][j] == 0;
			}
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				for (int k = 0;k < 4;k++) {
					int tox = i + dx[k], toy = j + dy[k];
					if (vis[i][j] == vis[tox][toy] && tox < n && toy < m) {
						
						int u = find(rk[i][j]), v = find(rk[tox][toy]);
						if (u == v)continue;
						fa[u] = v;size[v] += size[u];
					}
				}
			}
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (find(rk[i][j]) == rk[i][j] && size[rk[i][j]] >= 300 &&  vis[i][j] == 0)
					fa[rk[i][j]] = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (fa[find(rk[i][j])] == 0)
					vis[i][j] = 1;
	}
	void InitDSU() {
		int s = 0;
		fa[0] = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				rk[i][j] = ++s;
				fa[s] = s, size[s] = 1;
				maxn[s][0] = minu[s][0] = i;
				minu[s][1] = maxn[s][1] = j;
			}
	}
	ImageMatri ToBalckWhite(ImageMatri Target, string Type, string ResultPath, int redetect = 1) {
		//二值化并降噪
		flag = 1;
		memset(KeyPointCount, 0, sizeof(KeyPointCount));
		InitDSU();

		if (Type == "MISS") {
			GetKeyPoint(Target, Type);
			if (DEBUG)Target.SaveGrey("KeyMISS.PNG");
			if (KPC <= 200)return Target;
			ProcessFill(Target, Type);
			InitDSU();
			Merge(Target, Type, redetect);
		}
		if (Type == "YELLOW") {
			GetKeyPoint(Target, Type);
			if (DEBUG)Target.SaveGrey("KeyYELLOW.PNG");
			if (KPC <= 200)return Target;
			ProcessFill(Target, Type);
			if(DEBUG)ShowFill(Target);
			InitDSU();
			Merge(Target, Type, redetect);
		}
		if (Type == "NORMAL") {
			GetKeyPoint(Target, Type);
			if (DEBUG)Target.SaveGrey("KeyNORMAL.PNG");
			ProcessFill(Target, Type);
			InitDSU();
			if (KPC <= 200)return Target;
			Merge(Target, Type, redetect);
		}
		if (CONSOLERES && flag == 0 && redetect)Target.SaveGrey(ResultPath + "Res" + Type + ".PNG");
		return Target;
	}
}
int PreCheck(cv::Mat Img) {
	int r, g, b, is1 = 1, is2 = 1;
	int pos1[5][3] = { {325,941},{577,1011},{807,1081},{450,800} };
	int pos2[5][3] = { {325,1600 - 941},{577,1600 - 1011},{807,1600 - 1081},{450,800} };
	int resolution = Img.cols;
	for (int i = 0;i < 4;i++) {
		int x = pos1[i][0] * resolution / 1600, y = pos1[i][1] * resolution / 1600;
		r = (int)Img.at<cv::Vec3b>(x, y)[2], g = (int)Img.at<cv::Vec3b>(x, y)[1], b = (int)Img.at<cv::Vec3b>(x, y)[0];
		double h, s, v;
		rgb_to_hsv(r, g, b, h, s, v);
		if (!(h >= 0.47 && h <= 0.72 && v <= 0.98 && s >= 0.3 && v >= 0.4))
			is1 &= 0;
	}
	for (int i = 0;i < 3;i++) {
		int x = pos2[i][0] * resolution / 1600, y = pos2[i][1] * resolution / 1600;
		r = (int)Img.at<cv::Vec3b>(x, y)[2], g = (int)Img.at<cv::Vec3b>(x, y)[1], b = (int)Img.at<cv::Vec3b>(x, y)[0];
		double h, s, v;
		rgb_to_hsv(r, g, b, h, s, v);
		if (!(h >= 0.47 && h <= 0.72 && v <= 0.98 && s >= 0.3))
			is2 &= 0;
	}
	return is1 && is2;
}
int RecognizeNumber(ImageMatri& Img, string Type) {
	using namespace NoiseReduction;
	int failed = 0;
	MathMatri mat[100];int cnt = 0, allcnt = 0;
	int num = 0, limitmax = 0;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++) {
			int u = rk[i][j];
			if (find(u) != u || Img.g[i][j] == 0)continue;
			cmax(limitmax, maxn[u][0]);
		}
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++) {
			int u = rk[i][j];
			if (find(u) != u || Img.g[i][j] == 0)continue;
			if (limitmax - maxn[u][0] <= 10)
				mat[++cnt].Set(minu[u][0], maxn[u][0], minu[u][1], maxn[u][1]);
		}
	if (allcnt == 1 && Type == "YELLOW")failed = 1;
	sort(mat + 1, mat + cnt + 1);
	for (int i = 1;i <= cnt;i++) {
		NumberMatri obj;
		if (Type == "NORMAL") {
			obj.init(Img, mat[i].t, mat[i].b, mat[i].l, mat[i].r, 0);
		}
		else {
			if (cnt == 2 && i == 1) {
				obj.init(Img, mat[i].t, mat[i].b, mat[i].l, mat[i].r, 2);
			}
			else
				if (cnt == 3 && i == 1)
					obj.init(Img, mat[i].t, mat[i].b, mat[i].l, mat[i].r, 0);
				else
					obj.init(Img, mat[i].t, mat[i].b, mat[i].l, mat[i].r, 1);
		}
		if (DEBUG) {
			ImageMatri tmp = Img;
			auto img = tmp.ToGreyImage();
			tmp.ToMatri(img);
			img.release();
			tmp.crop(mat[i].l, mat[i].t, mat[i].r, mat[i].b);
			tmp.SaveGrey("Res" + NumberToString(i) + ".PNG");
		}
		num = num * 10 + obj.TestNumber();
	}
	ot << num << endl;
	printf("Number:%d\n", num);
	FightStatuNow += "Value:" + NumberToString(num) + ",Frame:" + NumberToString(FrameNow) + "\n";
	return failed;
}
void ReProcess(ImageMatri Img,string Type) {
	if (Type == "MISS") {
		NoiseReduction::Init(Img,50);//27ms
		NoiseReduction::ShowFill(Img);
		auto nimg = NoiseReduction::ToBalckWhite(Img, Type, NumberToString(FrameNow));
		if (flag == 1)printf("Boom!");
	}
	if (Type == "YELLOW") {
		NoiseReduction::Init(Img, 60);//27ms
		NoiseReduction::ShowFill(Img);
		auto nimg = NoiseReduction::ToBalckWhite(Img, Type, NumberToString(FrameNow));
		if (flag == 1)printf("Boom!");
		RecognizeNumber(nimg,Type);
	}
	if (Type == "NORMAL") {
		NoiseReduction::Init(Img, 45);//27ms
		NoiseReduction::ShowFill(Img);
		auto nimg = NoiseReduction::ToBalckWhite(Img, Type, NumberToString(FrameNow));
		if (flag == 1)printf("Boom!");
		RecognizeNumber(nimg, Type);
	}
}
void ProcessImage(cv::Mat img, string ResultPath = "") {
	printf("Time:%lf,Frame:%d\n",FrameNow/VideoRate,FrameNow);
	if (PreCheck(img) == 0) {
		printf("Skiped\n");
		return;
	}
	ImageMatri Img;

	Img.ToMatri(img);
	Img.resolution = img.cols;
	Img.resize(1600, 900);
	if (DEBUG)Img.SaveImage("Frame2.PNG");

	Img.crop(1039, 111, 1590, 741);
	if (DEBUG)Img.SaveImage("Frame.PNG");
	NoiseReduction::Init(Img,42);//27ms
	NoiseReduction::ShowFill(Img);

	//if (DEBUG)Debug::ShowProcessMemoryUsageInfo();
	NoiseReduction::SetBorder();
	auto nimg = NoiseReduction::ToBalckWhite(Img, "YELLOW", ResultPath,0);
	if (flag == 0) {
		ReProcess(Img,"YELLOW");
		if (CONSOLE)Img.SaveImage(NumberToString(FrameNow) + "Get.PNG");
		VideoFunction::SkipFrame(1.5);
		return;
	}
	NoiseReduction::SetBorder();
	nimg = NoiseReduction::ToBalckWhite(Img, "MISS", ResultPath, 0);
	if (flag == 0) {
		if (CONSOLE)Img.SaveImage(NumberToString(FrameNow) + "Get.PNG");
		VideoFunction::SkipFrame(1.5);
		ReProcess(Img, "MISS");
		return;
	}
	NoiseReduction::SetBorder();
	nimg = NoiseReduction::ToBalckWhite(Img, "NORMAL", ResultPath,0);
	if (flag == 0) {
		ReProcess(Img, "NORMAL");
		if (CONSOLE)Img.SaveImage(NumberToString(FrameNow) + "Get.PNG");
		VideoFunction::SkipFrame(1.5);
		return;
	}
	
	
	
}
int main() {
	
	
	InitMathMatri(MathMat, 1600);
	Debug::PrintWorkPath();
	//也可以将buffer作为输出参数

	double start = clock(), timeall = 0, cost, FightGap;
	ImageMatri Img;
	string VideoPath, ImagePath;
	int FrameCount;
	cv::Mat img;

	ifstream in("data.in");
	ofstream out("data.out");
	in >> VideoPath >> FightGap;

	cv::VideoCapture capture(VideoPath);
	//cout<<"resolution:" << capture.get(cv::CAP_PROP_XI_WIDTH) << endl;
	{
		capture.set(cv::CAP_PROP_POS_FRAMES, 0);
		capture >> img;
		printf("width:%d\n", img.cols);
		if (!capture.isOpened()) {
			std::printf("Error,Couldn't Open The File\n");
			return 0;
		}
		FrameCount = capture.get(cv::CAP_PROP_FRAME_COUNT);
		std::printf("FrameCount%d\n", FrameCount);
		VideoRate = capture.get(cv::CAP_PROP_FPS);
		VideoGap = 1000.0 / VideoRate;
		printf("FPS:%lf\n", VideoRate);
	}

	int framein = 0, lst = 0, NowFight = 1;
	FightStatuNow = "Fight1:\n";

	VideoFunction::SkipFrame(0);
	capture.set(cv::CAP_PROP_POS_FRAMES, FrameNow);
	while (1) {
		if (framein > 100) {
			framein = 0;
			capture.release();
			capture = cv::VideoCapture(VideoPath);
			int FrameCount = capture.get(cv::CAP_PROP_FRAME_COUNT);
		}
		cv::Mat img;
		capture.set(cv::CAP_PROP_POS_FRAMES, FrameNow);
		capture.read(img);

		if (FrameNow - lst >= FightGap * VideoRate) {
			out << FightStatuNow << "\n\n";
			NowFight++;
			FightStatuNow = "Fight" + NumberToString(NowFight) + ":\n";
			lst = FrameNow;
		}
		flag = 1;
		if (FrameNow / (FrameCount/20) > (FrameNow-5) / (FrameCount / 20))printf("Process:%d %% \n", FrameNow * 5 / (FrameCount / 20));
		ProcessImage(img, NumberToString(FrameNow));
		img.release();

		VideoFunction::SkipFrame(0.5);
		if (FrameNow >= FrameCount - 12)break;

	}
	out << FightStatuNow << endl;
	std::cout << "\n命中:" << StatuCount[1];
	std::cout << "\n暴击:" << StatuCount[2];
	std::cout << "\nMISS:" << StatuCount[3];
	std::printf("\nTotal Frames:%d\n", FrameCount);
	std::printf("FPS:%lf\n", VideoRate);
	capture.release();
	std::printf("TIME:%lf 秒", (clock() - start) / 1000);
	in.close();
	out.close();
	return 0;

}


