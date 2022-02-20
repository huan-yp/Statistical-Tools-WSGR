#include <opencv2\opencv.hpp>
#include <iostream>
#include <time.h>
#include <fstream>
#include <iostream>    
#include <windows.h>    
#include <psapi.h>    
#pragma comment(lib,"psapi.lib")    
using namespace std;

#define INF 1000000000


using namespace std;

int DEBUG = 0;
int TESTTIME = 0;
int CONSOLE = 0;
const int MAXWEIGHT=1605, MAXHEIGHT = 905;
const int BIRGHT = 18;
const int BOX[] = { {},{},{},{},{},{},{} };\

int flag = 1, skip,FrameNow;
int StatuCount[12];

double VideoGap, VideoRate;

string FightStatuNow;

struct Point {
	double x, y;
	Point() :x(), y() {}
	Point(double x, double y) :x(x), y(y) {}
	double CalcDis(const Point& a) {
		return sqrt(pow(a.x - x, 2) + pow(a.y - y, 2));
	}
	double CalcDis(double x, double y) {
		return CalcDis(Point(x, y));
	}
};
struct Line {
	double k, b;
	Line() :k(), b() {}
	Line(double k, double b) :k(k), b(b) {}
	void InitLineK(double kk, double bb) {
		k = kk, b = bb;
	}
	void InitLineP(Point a, Point bb) {
		k = (a.y - bb.y) / (a.x - bb.x);
		b = a.y - a.x * k;
	}
	double Calcdistance(Point p) {
		return abs(k * p.x - p.y + b) / sqrt(1 + k * k);
	}
	Point CalcIntersection(Line a) {
		Point res;
		res.x = (b - a.b) / (a.k - k);
		res.y = k * res.x + b;
		return res;
	}
};
const Line MISSLINE1 = Line(0.8598438026744578, -16.77396336667041);
const Line MISSLINE2 = Line(1.0137781000725163, -14.201957940536621);
const Line YELLOWLINE = Line(0.80546875, -75.34296875);

namespace MathFunction {
	double CalcDistance(double k, double b, double x, double y) {
		return Line(k, b).Calcdistance(Point(x, y));
	}
	double CalcED(Point a, Point b) {
		return a.CalcDis(b);
	}
	double CalcED(double x1, double y1, double x2, double y2) {
		return Point(x1, y1).CalcDis(Point(x2, y2));
	}
}

template<typename _type>
void cmax(_type& x, _type y) {
	x = max(x, y);
}
template<typename _type>
void cmin(_type& x, _type y) {
	x = min(x, y);
}
string NumberToString(int num) {
	if (num == 0)return "0";
	string res = "";
	res += char(num % 10 + '0');
	num /= 10;
	if (num == 0)return res;
	return NumberToString(num) + res;
}
struct ImageMatri {
	int n, m;
	int a[MAXHEIGHT][MAXWEIGHT][3], g[MAXHEIGHT][MAXWEIGHT];
	int resolution = 1600;
	string path;
	void resize(int weight, int height) {
		cv::Mat nimg;
		auto Img = ToImage();
		cv::resize(Img, nimg, cv::Size(weight, height));
		ToMatri(nimg);
		nimg.release(), Img.release();
	}
	void SetGrey() {
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				g[i][j] = (a[i][j][0] * 299 + a[i][j][1] * 587 + a[i][j][2] * 114 + 500) / 1000;
	}
	void ToMatri(cv::Mat& image, int is_rgb = 0) {
		n = image.rows, m = image.cols;
		for (int i = 0; i < n; ++i)
		{
			//获取第i行首像素指针
			cv::Vec3b* p = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < m; ++j)
			{
				//将img的bgr转为image的rgb 
				a[i][j][2] = p[j][0];
				a[i][j][1] = p[j][1];
				a[i][j][0] = p[j][2];
			}
		}
		SetGrey();
	}
	void read(string ph) {
		//从 path 读取图片，并转化为 RGB通道值保存，同时计算灰度
		path = ph;
		cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
		ToMatri(image);
		resolution = image.cols;
		image.release();
	}
	cv::Mat ToImage() {
		cv::Mat image(n, m, CV_8UC3);
		for (int i = 0; i < n; ++i)
		{
			//获取第i行首像素指针
			cv::Vec3b* p = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < m; ++j)
			{
				p[j][0] = a[i][j][2];
				p[j][1] = a[i][j][1];
				p[j][2] = a[i][j][0];
			}
		}
		return image;
	}
	cv::Mat ToGreyImage() {
		cv::Mat image(n, m, CV_8UC3);
		for (int i = 0; i < n; ++i)
		{
			//获取第i行首像素指针
			cv::Vec3b* p = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < m; ++j)
			{
				p[j][0] = g[i][j];
				p[j][1] = g[i][j];
				p[j][2] = g[i][j];
			}
		}
		return image;
	}
	void SaveImage(string path) {
		//将图片保存至 path
		auto img = ToImage();
		cv::imwrite(path, img);
		img.release();
	}
	void SaveGrey(string path) {
		auto img = ToGreyImage();
		cv::imwrite(path, img);
		img.release();
	}
	void crop(int left, int top, int right, int buttom) {
		auto Img = ToImage();
		auto nimg = Img(cv::Rect(left, top, right - left, buttom - top));
		ToMatri(nimg);
		nimg.release();
		Img.release();
	}
};
struct MathMatri {
	int l, r, b, t;
	Point cen;
	void Set(int tt, int bb, int ll, int rr) {
		l = ll, r = rr, b = bb, t = tt;
		cen = Point(b + t >> 1, l + r >> 1);
	}
	bool in(int x, int y) {
		return x >= t && x <= b && y <= r && y >= l;
	}
}MathMat[12];
struct ImageGraph {
	//图像特征点离散图,用于特征点匹配。
	//暂时没想到怎么写，公式还没推出来，不过不做复杂情景识别就不急
	//也还没有数据
	vector<int> e[100];
	double GetGraphFromMatri(const ImageMatri& Image) {

	}
	double CalcSimilarity(const ImageMatri& Image) {

	}

};
namespace NoiseReduction {
	//针对不同情景下的降噪
	const int N = 1605, M = 1605;
	const int dx[] = { 1,1,0,0 }, dy[] = { 1,0,1,0 };
	int Border = 20, n, m, cnt = 0, s = 0;
	int rk[N][M], fa[N * M], size[N * M], KeyPointCount[10];
	int maxn[N * M][2], minu[N * M][2], vis[N][M];
	bool border[N][M], in[N][M];
	queue<pair<int, int>> q;
	//vis[i][j]:-1->未计算,0:在边框内,1:不在边框内
	//淡化图像边界提取效果很差，边界提取算法需改进
	bool ok(int x, int y) {
		return x >= 0 && y >= 0 && x < n&& y < m;
	}
	void bfs() {
		//这里挂了，dfs 搜不出来
		//rewiting...
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				if (border[i][j])
					vis[i][j] = 0;
		for (int i = 0;i < n;i++) {
			if (vis[i][0] == -1)q.push(make_pair(i, 0)), vis[i][0] = 1;
			if (vis[i][m] == -1)q.push(make_pair(i, m)), vis[i][m] = 1;
		}
		for (int i = 0;i < m;i++) {
			if (vis[n][i] == -1)q.push(make_pair(n, i)), vis[n][i] = 1;
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
		if(DEBUG)img.SaveGrey("Border.PNG");
	}
	bool CheckBorder(const int* a) {
		double r = a[0], g = a[1], b = a[2];
		if ((abs(r - g) <= 60 && abs(g - b) <= 60 && abs(r - b) <= 60))
			return 1;
		if (min(r, min(g, b)) == 0)
			return 0;
		double flite1 = r / b, flite2 = b / g, flite3 = g / r;
		if (flite1 <= 1.4 && flite1 >= 0.7 && flite2 <= 1.4 && flite2 >= 0.7 && flite3 <= 1.4 && flite3 >= 0.7)
			return 1;
		return 0;
	}
	bool CheckWhite(const int* a) {
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
				if (Target.g[i][j]==0)count++;
			}
		if (DEBUG)Target.SaveGrey("Fill.PNG");
		if (count == 0 || count > n * m / 3)
			skip = 1;
	}
	void GetKeyPoint(ImageMatri& Target, Line l1, Line l2, string Type, double DisLimit = 18) {
		//传入一个图形矩阵，分析特征点并写入灰度值
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				if (Type == "MISS") {
					Target.g[i][j] = 0;
					if (Target.a[i][j][0] < 170 || Target.a[i][j][1] > 175 || Target.a[i][j][1] < 40)continue;
					else {}
					double dis1 = l1.Calcdistance(Point(Target.a[i][j][1], Target.a[i][j][2]));
					double dis = INF;
					double dis2 = l2.Calcdistance(Point(Target.a[i][j][1], Target.a[i][j][2]));
					if (dis1 > dis2)swap(dis1, dis2);
					dis = dis1 * 0.9 + dis2 * 0.1;
					if (dis <= DisLimit)
						Target.g[i][j] = 255;

					else
						Target.g[i][j] = 0;
				}
				if (Type == "YELLOW") {
					Target.g[i][j] = 0;
					if (Target.a[i][j][0] < 170 || Target.a[i][j][1] < 150)continue;
					else {}
					Point p = Point(Target.a[i][j][1], Target.a[i][j][2]);
					double dis = l1.Calcdistance(Point(Target.a[i][j][1], Target.a[i][j][2]));
					if (dis <= DisLimit)
						Target.g[i][j] = 255;
					else
						Target.g[i][j] = 0;
				}
				if (Type == "NORMAL") {
					//if (vis[i][j] == 0)g[i][j] = 255;
					//else g[i][j] = 0;

					if (Target.g[i][j] <= 128) {
						Target.g[i][j] = 0;
						continue;
					}
					if (CheckWhite(Target.a[i][j]))
						Target.g[i][j] = 255;
					else
						Target.g[i][j] = 0;
				}
			}
	}
	void GetBorder(const ImageMatri& Target) {
		skip = 0;
		int BorderCount = 0;
		Border = 1.0 * 35 / (1.0 * Target.resolution / 1600);
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				border[i][j] = Target.g[i][j] <= Border && CheckBorder(Target.a[i][j]);
				BorderCount += border[i][j];
			}
		if (DEBUG)ShowBorder();
		if (BorderCount >= n * m /7)skip = 1;
	}
	void Merge(ImageMatri& Target, string Type) {
		//合并关键点
		cnt = 0;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				for (int k = 0;k < 4;k++) {
					int tox = i + dx[k], toy = j + dy[k];
					if (tox >= n || toy >= m || Target.g[i][j] != Target.g[tox][toy])continue;
					merge(rk[i][j], rk[tox][toy]);
				}
			}

		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				int u = find(rk[i][j]);
				int height = maxn[u][0] - minu[u][0], weight = maxn[u][1] - minu[u][1];
				if (Type == "MISS") {
					if (size[u] <= 75 || height <= 15 || weight <= 15 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (Type == "YELLOW") {
					if (size[u] <= 200 || height <= 15 || weight <= 15 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (Type == "NORMAL") {
					if (size[u] >= 500 || size[u] <= 50 || height <= 10 || weight <= 5 || vis[i][j] == 1)
						Target.g[i][j] = 0;
				}
				if (u == rk[i][j] && Target.g[i][j]) {
					cnt++;
					for (int k = 1;k <= 6;k++) {
						if (MathMat[k].in(i, j))
							KeyPointCount[k]++;
					}
				}
			}
		if (cnt) {
			flag = 0;
			printf("Type:");
			if (Type == "MISS")StatuCount[3]++,std::cout<<Type<<",Amount:"<< StatuCount[3]<<endl;
			if (Type == "YELLOW")StatuCount[2]++,std::cout << Type << ",Amount:" << StatuCount[2] << endl;
			if (Type == "NORMAL")StatuCount[1]++,std::cout << Type << ",Amount:" << StatuCount[1] << endl;
		}
		for (int k = 1;k <= 6;k++) {
			if (KeyPointCount[k] >= 1)
				FightStatuNow += "Position:"+NumberToString(k)+",Type:"+Type+"\n";
		}
	}
	void Init(ImageMatri& Target) {
		n = Target.n, m = Target.m;
		s = 0;
		GetBorder(Target);
		//初始化
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++)
				vis[i][j] = -1;
		//提取边框
		bfs();
	}
	int ToBalckWhite(ImageMatri Target, string Type) {
		//二值化并降噪
		memset(KeyPointCount, 0, sizeof(KeyPointCount));
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				rk[i][j] = ++s;
				fa[s] = s, size[s] = 1;
				maxn[s][0] = minu[s][0] = i;
				minu[s][1] = maxn[s][1] = j;
			}

		if (Type == "MISS") {
			GetKeyPoint(Target, MISSLINE1, MISSLINE2, Type);
			if (DEBUG)Target.SaveGrey("KeyMISS.PNG");
			Merge(Target, Type);
		}
		if (Type == "YELLOW") {
			GetKeyPoint(Target, YELLOWLINE, YELLOWLINE, Type);
			if (DEBUG)Target.SaveGrey("KeyYELLOW.PNG");
			Merge(Target, Type);
		}
		if (Type == "NORMAL") {
			GetKeyPoint(Target, YELLOWLINE, YELLOWLINE, Type);
			if (DEBUG)Target.SaveGrey("KeyNORMAL.PNG");
			Merge(Target, Type);
		}
		if(DEBUG)Target.SaveGrey("Res" + Type + ".PNG");
		return 0;
	}
}
namespace Split {
	//提取有效部分
}
namespace VideoFunction {
	void SkipFrame(double second) {
		FrameNow += second * 1000.0 / VideoGap;
	}
}
namespace Debug {
	void ShowProcessMemoryUsageInfo()
	{
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
		float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
		float memoryUsage_K = pmc.WorkingSetSize / 1024.0;

		std::cout << std::fixed << std::setprecision(2) << "内存使用:" << memoryUsage_K << "K " << memoryUsage_M << "M" << std::endl;
	}

}
void ProcessImage(ImageMatri &Img) {
	Img.resize(1600, 900);
	Img.crop(1039, 111, 1565, 741);
	if (DEBUG)Img.SaveImage("Frame.PNG");
	flag = 1;
	if(DEBUG)printf("FrameNow:%d\n", FrameNow);
	NoiseReduction::Init(Img);
	NoiseReduction::ShowFill(Img);
	if(DEBUG)Debug::ShowProcessMemoryUsageInfo();
	if (skip) {
		VideoFunction::SkipFrame(0.5);
		return;
	}
	auto nimg = NoiseReduction::ToBalckWhite(Img, "NORMAL");

	if (flag)nimg = NoiseReduction::ToBalckWhite(Img, "YELLOW");

	if (flag)nimg = NoiseReduction::ToBalckWhite(Img, "MISS");

	if (flag==0&&CONSOLE)Img.SaveImage(NumberToString(FrameNow)+"Get.PNG");
}

int main() {
	//cv::VideoCapture video("chaplin.mp4");
	//需要实现的功能：
	//从工作目录下 data.in 中读取需要处理的图片路径列表
	//输出结果到 data.out
	//结果的形式为每张图片一个六元组
	//具体功能：
	//读取一张 16:9 完整游戏图片并进行未命中，跳弹，暴击，命中的判定
	// 
	//实现思路：
	//1.降噪：
	//对于跳弹和未命中和暴击:
	//(1)对目标颜色区域建立回归直线方程，取固定阈值为是否是关键点的第一个判段依据
	//(2)建立边框色彩特征直线方程，取基于分辨率的动态阈值作为判断是否为边框的依据
	//(3)按照边框对像素点进行划分，以是否在边框内作为是否为关键点的第二个判断依据
	//(4)按照是否为关键点二值化，将每个关键点块矩阵化并消除椒盐噪点
	// 2.识别：
	//(1)简单情景识别：判断是否存在矩阵块
	//(2)复杂情景识别：
	// 
	//
	//<1>将关键部分作为矩阵提取出来，按照位置排序
	//<2>描绘出每个关键部分的轮廓，提取特征点进行匹配
	//特征点选取和匹配怎么做还没想好
	//
	//
	/*
	int type;
	while (cin >> type) {
		cout << NumberToString(type) << endl;
	}
	string path = "C:\\Users\\Administrator\\Desktop\\Counts\\rec2\\10.PNG";
	printf("请输入图片路径\n");
	cin >> path;
	string Type;

	printf("请选择类型(1为命中滤镜，2为暴击滤镜)");
	cin >> type;
	if (type == 1)Type = "MISS";
	else Type = "YELLOW";

	ImageMatri Image;
	Image.read(path);
	*/
	{
		//Img.crop(1039, 111, 1565, 741);
		MathMat[1].Set(555 - 111, 715 - 111, 1142 - 1039, 1382 - 1039);
		MathMat[2].Set(499 - 111, 665 - 111, 1396 - 1039, 1590 - 1039);
		MathMat[3].Set(377 - 111, 544 - 111, 1137 - 1039, 1354 - 1039);
		MathMat[4].Set(307 - 111, 473 - 111, 1341 - 1039, 1562 - 1039);
		MathMat[5].Set(227 - 111, 365 - 111, 983 - 1039, 1235 - 1039);
		MathMat[6].Set(139 - 111, 307 - 111, 1221 - 1039, 1465 - 1039);
	}

	double start = clock(), timeall = 0, cost, FightGap;
	ImageMatri Img;
	//open video
		
	string VideoPath;
	ifstream in("data.in");
	ofstream out("data.out");
	in >> VideoPath >> FightGap;
	cv::VideoCapture capture(VideoPath);
	if (!capture.isOpened()) {
		std::printf("Error,Couldn't Open The File\n");
		return 0;
	}
	int FrameCount = capture.get(cv::CAP_PROP_FRAME_COUNT);
	std::printf("FrameCount%d\n", FrameCount);

	VideoRate = capture.get(cv::CAP_PROP_FPS);
	VideoGap = 1000 / VideoRate;
	printf("FPS:%lf\n",VideoRate);
	VideoFunction::SkipFrame(0);
	int framein = 0, lst = 0,NowFight=1;
	FightStatuNow = "Fight1:\n";
	

	while (1) {
		if (framein > 10) {
			framein = 0;
			capture.release();
			capture = cv::VideoCapture(VideoPath);
			int FrameCount = capture.get(cv::CAP_PROP_FRAME_COUNT);
		}
		cv::Mat img;
		ImageMatri Img;
		capture.set(cv::CAP_PROP_POS_FRAMES, FrameNow);
		capture >> img;
		if (FrameNow-lst >= FightGap * VideoRate) {
			out << FightStatuNow << "\n\n";
			NowFight++;
			FightStatuNow = "Fight"+NumberToString(NowFight)+":\n";
			lst = FrameNow;
		}
		Img.ToMatri(img);
		img.release();
		ProcessImage(Img);

		VideoFunction::SkipFrame(0.5);
		if (flag == 0) {
			lst = FrameNow;
			if(DEBUG)std::printf("Get at %lf\n", 1.0 * FrameNow / VideoRate);
			VideoFunction::SkipFrame(1);
		}
		if (FrameNow >= FrameCount)break;

	}
	out << FightStatuNow << endl;
	std::cout<<"\n命中:" <<StatuCount[1];
	std::cout<<"\n暴击:"<< StatuCount[2];
	std::cout<<"\nMISS:"<< StatuCount[3];
	std::printf("Total Frames:%d\n", FrameCount);
	std::printf("FPS:%lf\n", VideoRate);
	capture.release();
	std::printf("TIME:%lf", clock() - start);
	in.close();
	out.close();
	return 0;
	
}
