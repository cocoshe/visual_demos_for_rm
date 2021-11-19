/*
	关于RotatedRect中的角度探究，得出以下结论：
	1.fitEllipes函数返回的RotatedRect：一定是:
		height > width;
		0 <= angle <= 180°;
		angle指width与x轴的夹角(角永远是在x轴下方，恒为正)
	2.adjustRec对矩形进行矫正，因为两个差不多平行的装甲板，会因为其左偏or右偏一丢丢而有很大的角度差
	但是ANGLE_TO_UP会在45度附近交换width和heigh，从而失效！
*/


#include "opencv2/opencv.hpp"
#include <iostream>
#define PI 3.141592
#define WIDTH_GREATER_THAN_HEIGHT 0
#define ANGLE_TO_UP 1

using namespace std;
using namespace cv;


void Draw(Mat& image, vector<Point2f>& p);							//draw contours
RotatedRect DrawRotatedRect(Mat& image, vector<Point2f>& p);		//draw RotatedRect of fitEllipes()
void onChangeTrackBar(int pos, void* usrdata);						//call back function
void Rotate(Point2f& Center, vector<Point2f> &Points, int angle);	//rotate some points(math)
RotatedRect& adjustRec(cv::RotatedRect& rec, const int mode);		//adjust the rotatedrect


vector<Point2f> P_FOR_ALL = { {100, 100}, {200, 150}, {300, 300}, {350, 350}, {250, 350} };	//全局的点
Point2f Center = {300, 300};	//旋转中心


int main() {
	string trackBarName = "角度：";
	int posTrackBar = 0;	//TrackBar初值
	int maxValue = 360;		//TrackBar上限


	Mat image1(500, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat image2(500, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat image2bk;	//备份
	image2bk = image2.clone();

	vector<Point2f> p = { {100, 100}, {200, 150}, {300, 300}, {350, 350}, {250, 350}};

	Draw(image1, p);								//自己乱画一个多边形

	RotatedRect rec1 = DrawRotatedRect(image1, p);	//画椭圆拟合得到的RotatedRect

	imshow("origin", image1);
	cout << "\n\n\n" << endl;
	cout << "RotatedRect1的width：" << rec1.size.width << endl;
	cout << "RotatedRect1的height：" << rec1.size.height << endl;
	cout << "RotatedRect1的angle：" << rec1.angle << endl;

	//镜像一下，观察RotatedRect
	for (int i = 0; i < 5; i++) {
		p[i].x = image1.cols - p[i].x;
		p[i].y += 20;
	}

	Draw(image2, p);								//自己乱画一个多边形

	RotatedRect rec2 = DrawRotatedRect(image2, p);	//画椭圆拟合得到的RotatedRect

	imshow("change_dir", image2);
	
	cout << "\n\n\n" << endl;
	cout << "RotatedRect2的width：" << rec2.size.width << endl;
	cout << "RotatedRect2的height：" << rec2.size.height << endl;
	cout << "RotatedRect2的angle：" << rec2.angle << endl;



	createTrackbar(trackBarName, "change_dir", &posTrackBar, maxValue, onChangeTrackBar, &image2bk);

	waitKey(0);
}


/* 连接各个点 */
void Draw(Mat& image, vector<Point2f>& p) {
	for (int i = 0; i < 5; i++) {
		line(image, p[i], p[(i + 1) % 5], Scalar(0, 0, 255));
	}
}

/*
*	画椭圆拟合的矩形
*/
RotatedRect DrawRotatedRect(Mat& image, vector<Point2f>& p) {
	RotatedRect rec = fitEllipse(p);
	Point2f pp[4];
	rec.points(pp);
	for (int i = 0; i < 4; i++) {
		line(image, pp[i], pp[(i + 1) % 4], Scalar(255, 0, 0));
	}
	return rec;
}




/* 回调函数 */
void onChangeTrackBar(int pos, void* usrdata) {
	Mat src = *(Mat*)(usrdata);

	Mat dst;
	
	vector<Point2f> temp(P_FOR_ALL);

	Rotate(Center, temp, pos);
	dst = src.clone();
	Draw(dst, temp);
	RotatedRect rec = DrawRotatedRect(dst, temp);
	cout << "----------------------------------------------" << endl;
	cout << "\n" << endl;
	cout << "before adjustRec()" << endl;
	cout << "RotatedRect的width：" << rec.size.width << endl;
	cout << "RotatedRect的height：" << rec.size.height << endl;
	cout << "RotatedRect的angle：" << rec.angle << endl;

	rec = adjustRec(rec, ANGLE_TO_UP);					
//	rec = adjustRec(rec, WIDTH_GREATER_THAN_HEIGHT);	//差不多倾斜程度，但是角度可能相差一个符号：-89可能和89是同一个装甲板的灯条

	cout << "\n" << endl;
	cout << "after adjustRec()" << endl;
	cout << "RotatedRect的width：" << rec.size.width << endl;
	cout << "RotatedRect的height：" << rec.size.height << endl;
	cout << "RotatedRect的angle：" << rec.angle << endl;

	namedWindow("change_dir", WINDOW_NORMAL);
	imshow("change_dir", dst);
}

/*
* input:
*	Center	旋转中心点
*	Points	待转动的点&
*	angle	转动角度
*/
void Rotate(Point2f& Center, vector<Point2f>& Points, int angle) {
	for (int i = 0; i < Points.size(); i++) {
		float dx = Points[i].x - Center.x;
		float dy = Points[i].y - Center.y;
		Points[i].x = Center.x + cos(angle / 180.0 * PI) * dx - sin(angle / 180.0 * PI) * dy;
		Points[i].y = Center.y + sin(angle / 180.0 * PI) * dx + cos(angle / 180.0 * PI) * dy;
	}
}



/*
*	RM中的adjustRec
*	mode:
*	WIDTH_GREATER_THAN_HEIGHT	0
*	ANGLE_TO_UP					1
*/
RotatedRect& adjustRec(cv::RotatedRect& rec, const int mode)
{
	using std::swap;

	float& width = rec.size.width;
	float& height = rec.size.height;
	float& angle = rec.angle;

	if (mode == WIDTH_GREATER_THAN_HEIGHT)
	{
		if (width < height)
		{
			swap(width, height);
			angle += 90.0;
		}
	}

	while (angle >= 90.0)
		angle -= 180.0;
	while (angle < -90.0)
		angle += 180.0;

	if (mode == ANGLE_TO_UP)
	{
		if (angle >= 45.0)
		{
			swap(width, height);
			angle -= 90.0;
		}
		else if (angle < -45.0)
		{
			swap(width, height);
			angle += 90.0;
		}
	}
	return rec;
}
