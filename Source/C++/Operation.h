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
#include <opencv.hpp>
const int MAXWEIGHT = 2305, MAXHEIGHT = 1305;
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
	short a[MAXHEIGHT][MAXWEIGHT][3], g[MAXHEIGHT][MAXWEIGHT];
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
		n = image.rows, m = image.cols + 2;
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
	friend bool operator <(const MathMatri& a, const MathMatri& b) {
		return a.l < b.l;
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
