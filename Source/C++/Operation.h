/*

*/



#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <iostream>    
#include <windows.h>    
#include <psapi.h>    
#include <direct.h> 
#include <stdlib.h>
#include <string.h>
#include <opencv2\opencv.hpp>
const int MAXWEIGHT = 2305, MAXHEIGHT = 1605;
using namespace std;
template<typename _type>
void cmax(_type& x, _type y) {
	x = max(x, y);
}
template<typename _type>
void cmin(_type& x, _type y) {
	x = min(x, y);
}
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
		string path = ph;
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
struct NumberMatri {
	/*
	检测算法：
	直线特征匹配+联通性质匹配
	对目标建立若干直线，按照关键点多少对直线分类，建立 01 特征向量并匹配。
	*/
	int a[100][100],n,m,cover,all=0,block=0;
	int rk[100][100], fa[100*100],size[100*100],bl[100];
	int maxn[100 * 100][2], minu[100 * 100][2];
	int line[100];
	int find(int x) {
		return fa[x] == x?x:fa[x] = find(fa[x]);
	}
	void merge(int u, int v) {
		u = find(u), v = find(v);
		if (u == v)return;
		size[v] += size[u];
		cmax(maxn[v][0],maxn[u][0]), cmax(maxn[v][1], maxn[u][1]);
		cmin(minu[v][0],minu[u][0]), cmin(minu[v][1], minu[u][1]);
		fa[u] = v;
	}
	void init(ImageMatri Img,int t,int b,int l,int r) {
		const int dx[]={1,-1,0,0}, dy[] = {0,0,1,-1};
		n = b - t + 3, m = r - l + 3 ;
		int s = 0;
		cover = n <= 31;
		for (int i = t;i <= b;i++)
			for (int j = l;j <= r;j++)
				a[i - t + 1][j - l + 1] = Img.g[i][j]/255;
		for(int i=0;i<n;i++)
			for (int j = 0;j < m;j++) {
				all += a[i][j];
				rk[i][j] = ++s;
				fa[s] = s, size[s] = 1;
				maxn[s][0] = minu[s][0] = i;
				maxn[s][1] = minu[s][1] = j;
			}
		//二次降噪
		for(int times=1;times<=2;times++)
			for(int i=0;i<n;i++)
				for (int j = 0;j < m;j++) {
					if (i == 0 || j == 0 || i == n - 1 || j == m - 1)continue;
					int cnt = 0;
					for (int k = 0;k < 4;k++) {
						int tox=i+dx[k], toy = j+dy[k];
						cnt += a[tox][toy];
					}
					if (cnt > 2)a[i][j] = 1;
					if (cnt < 2)a[i][j] = 0;
				}
		//提取孤立点
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				line[i] += a[i][j];
				if (i == 0 || j == 0 || i == n - 1 || j == m - 1)continue;
				int cnt = 0;
				for (int k = 0;k < 4;k++) {
					int tox = i + dx[k], toy = j + dy[k];
					if (a[i][j] == a[tox][toy])
						merge(rk[i][j], rk[tox][toy]);
				}
			}
		for (int i = 0;i < n * m;i++) {
			if (find(i) == i)bl[++block]=i;
		}
	}
	int TestNumber() {
		//先去掉 1
		//检验数字
		if (m <= 14)return 1;
		if (cover) {
			//被遮挡的匹配方式
			//检验连通块大小判断 7
			if (all <= 150)return 7;
			//连通块个数筛出 6,8
			if (block == 3) {
				//检查上部特征直线判断 6,8
				int SwitchLine = 0;
				for (int i = 2;i <= n / 2 - 4;i++) {
					int SwitchTimes = 0;
					for (int j = 0;j < m;j++){
						if (j+1 == m||a[i][j]==a[i][j+1])continue;
						SwitchTimes++;
					}
					if (SwitchTimes == 4)SwitchLine ++;
				}
				if (SwitchLine)return 8 ;
				return 6;
			}
			int SwitchLine = 0;
			for (int i = 0;i <= n / 2 - 2;i++) {
				int SwitchTimes = 0;
				for (int j = 0;j < m;j++) {
					if (j + 1 == m || a[i][j] == a[i][j + 1])continue;
					SwitchTimes++;
				}
				if (SwitchTimes == 4)SwitchLine++;
			}
			int u = find(1), Target = bl[1] == u ? 2 : 1;Target = bl[Target];
			if (SwitchLine) {
				//纵向直线切割区分 4,9
				int maxpos=0;
				for (int i = 0;i < n;i++)
					for (int j = 0;j <= n;j++)
						if (a[i][j]&&j==minu[Target][1])
							cmax(maxpos, i);
							if (maxpos >= n / 2 + 2)return 4;
				return 9;
			}
			//检查所有特征直线区分 2,3,5
			int SwitchTime = 0;
			for (int i = 0;i < n;i++) {
				if (i + 1 == n - 1 || (line[i]<=12)==(line[i+1]<=12))continue;
				SwitchTime++;
			}
			if (SwitchTime<4)return 2;
			//检查最上面的特征直线区分 3,5;
			int first = m, last = 0;
			for (int i = 0;i <= n;i++){
				for (int j = 0;j < m;j++)
					if (i == minu[Target][0] && a[i][j])
						cmin(first, j), cmax(last,j);
			}
			if (first <= m / 2)return 5;
			if (last >= m / 2)return 3;

		}
		else {
			//没被遮挡的匹配方式
			//检查连通块，判断 8，6，9，0
			if (block == 4)return 8;

			if (block == 3) {
				//4 的上部如果过于模糊，会成为一个连通块
				//注意这里可能把 4 判断进去
				int siz = min(size[bl[1]],min(size[bl[2]],size[bl[3]])),rk=0;
				//检查空心大小并判断 0 
				if (siz > 80)return 0;
				for (int i = 1;i <= 3;i++)if (size[bl[i]]==siz)rk = bl[i];
				//检查特征直线判断 4
				if (maxn[rk][0] - minu[rk][1] > 10)return 4;
				//检查联通块位置判断 6,9
				int pos = maxn[rk][0]+minu[rk][0]>>1;
				if (pos >= n / 2)return 6;
				return 9;
				//9,0,6
			}
			//检查所有特征直线，判断 2,4
			int SwitchLine = 0;
			for (int i = 0;i < n / 2 + 3;i++) {
				int SwitchTimes = 0;
				for (int j = 0;j < m;j++) {
					if (a[i][j] == a[i][j+1] || j + 1 == m)continue;
					SwitchTimes++;
				}
				if (SwitchTimes == 4)SwitchLine++;
			}
			if (SwitchLine) {
				if (SwitchLine > 4)return 4;
				return 2;
			}
			int SwitchTimes = 0;
			int pos = n / 2 - 4,i;
			for (i = n - 1;i >= n / 2 - 3;i--) {
				if (line[i] >= 12)break;
			}
			//检验下部特征直线判断 7
			if (i < n / 2 - 3) return 7;
			
			//检验上部特征直线判断 3,5
			for (i = 0;i < n;i++) {
				int first=m, last = 0;
				if (line[i] < 12) {
					for (int j = 0;j <= m;j++)
						if (a[i][j])
							cmin(first, j), cmax(last, j);
				}
				if (first <= n / 2 - 2)return 5;
				if (last >= n / 2 + 2)return 3;
			}
		}
		printf("Boom!!!\n");
		return -1;
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
};
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
namespace MathFunction {
	double CalcDistance(double k, double b, double x, double y) {
		return Line(k, b).Calcdistance(Point(x, y));
	}
	double CalcE2D(Point a, Point b) {
		return a.CalcDis(b);
	}
	double CalcE2D(double x1, double y1, double x2, double y2) {
		return Point(x1, y1).CalcDis(Point(x2, y2));
	}
	double CalcE3D(double x1, double y1, double z1, double x2, double y2, double z2) {
		return sqrt(pow(x1-x2,2)+pow(y1-y2,2)+pow(z1-z2,2));
	}
}
void InitMathMatri(struct MathMatri MathM[], int weight) {
	//Img.crop(1039, 111, 1565, 741);
	MathM[1].Set((555 - 111) / 1600.0 * weight, (715 - 111) / 1600.0 * weight, (1142 - 1039) / 1600.0 * weight, (1382 - 1039) / 1600.0 * weight);
	MathM[2].Set((499 - 111) / 1600.0 * weight, (665 - 111) / 1600.0 * weight, (1396 - 1039) / 1600.0 * weight, (1590 - 1039) / 1600.0 * weight);
	MathM[3].Set((377 - 111) / 1600.0 * weight, (544 - 111) / 1600.0 * weight, (1137 - 1039) / 1600.0 * weight, (1354 - 1039) / 1600.0 * weight);
	MathM[4].Set((307 - 111) / 1600.0 * weight, (473 - 111) / 1600.0 * weight, (1341 - 1039) / 1600.0 * weight, (1562 - 1039) / 1600.0 * weight);
	MathM[5].Set((223 - 111) / 1600.0 * weight, (365 - 111) / 1600.0 * weight, (1039 - 1039) / 1600.0 * weight, (1235 - 1039) / 1600.0 * weight);
	MathM[6].Set((139 - 111) / 1600.0 * weight, (307 - 111) / 1600.0 * weight, (1221 - 1039) / 1600.0 * weight, (1465 - 1039) / 1600.0 * weight);
}
void rgb_to_hsv(int R, int G, int B, double& H, double& S, double& V)
{
	UCHAR b = B, g = G, r = R;
	UCHAR max, min, tmp;
	float f_tmp = 0;
	tmp = b > g ? b : g;
	max = tmp > r ? tmp : r;//取得最大值
	if (max == 0) {
		H = S = V = 0;
		return;
	}
	tmp = b > g ? g : b;
	min = tmp > r ? r : tmp;//取得最小值
	if (max == min)
		goto here;
	H = r - g;
	S = r - b;
	V = g - b;
	f_tmp = acos((H + S) / 2 / sqrt(H * H + S * V)) * 180 / 3.1415926;
	if (b <= g)
		H = f_tmp;
	else H = 360 - f_tmp;
	H /= 360;
	here:
		S = (max - min) / static_cast<float>(max);//s
	V = max / 255.0;//v
}


