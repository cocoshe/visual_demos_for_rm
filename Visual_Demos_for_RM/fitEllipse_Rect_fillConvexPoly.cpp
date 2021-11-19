#include "opencv.hpp"
#include <iostream>

using namespace cv;

/****************
区分RotatedRect和普通Rect区别：
前者可以旋转，后者不能旋转。
另外，Rectangle函数直接拿来画Rect，用法与line函数画直线一样

fillConvexPoly函数：
功能：利用顶点来填充一个多边形
参数：选画板；输入多边形顶点vector；颜色
*****************/


int main() {
	Mat image(200, 200, CV_8UC3, Scalar(0));
	RotatedRect r_rect(Point2f(100, 100), Size2f(100, 50), 60);

	Point2f p[4];
	r_rect.points(p);

	//画RotatedRect
	for (int i = 0; i < 4; i++) {
		line(image, p[i], p[(i + 1) % 4], Scalar(0, 0, 255));
	}

	//取外接矩形，用rectangle函数画外接Rect
	Rect rect = r_rect.boundingRect();
	rectangle(image, rect, Scalar(0, 255 ,0));

	Mat lightMask = Mat::zeros(rect.size(), CV_8UC1);

	r_rect.points(p);
	std::vector<Point> RectPoints;

	/*
		下面的循环就相当于求出了RotatedRect对外接Rect的左上角的相对位置
		
		其实可以当作就是取了一个外接Rect的ROI
		然后在全黑的Mask里面
		用新得到的RotatedRect相对Rect的顶点进行多边形填充，颜色为255白色
	*/
	for (int i = 0; i < 4; i++)
	{
		RectPoints.emplace_back(Point(p[i].x - rect.tl().x,
			p[i].y - rect.tl().y));
	}
	fillConvexPoly(lightMask, RectPoints, 255);		//在全黑Mask里面画出白色的rotated矩阵
	imshow("fill", lightMask);						//显示自己画的Mask
	imshow("r_rect", image);						//显示原来的预处理的真实图像
	waitKey(0);
}







