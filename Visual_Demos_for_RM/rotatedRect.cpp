#include "opencv.hpp"

using namespace cv;


/*****************************
测试RotatedRect中angle的性质：
	1.顺时针转动为+
	2.逆时针转动为-
	3.不管angle的范围(虽然官方是把angle的范围定在-90°到0°的)
******************************/


int main() {
	Mat image(200, 200, CV_8UC3, Scalar(0));
	RotatedRect rect(Point2f(100, 100), Size2f(100, 50), 30);//中心点，width height，angle

	Point2f p[4];
	rect.points(p);
	for (int i = 0; i < 4; i++) {
		line(image, p[i], p[(i + 1) % 4], Scalar(0, 0, 255));
	}

	rect.angle += 40;

	rect.points(p);
	for (int i = 0; i < 4; i++) {
		line(image, p[i], p[(i + 1) % 4], Scalar(0, 255, 0));
	}
	
	swap(rect.size.width, rect.size.height);
	rect.points(p);
	for (int i = 0; i < 4; i++) {
		line(image, p[i], p[(i + 1) % 4], Scalar(255, 0, 0));
	}

	imshow("rectangles", image);
	waitKey(0);


	return 0;
}
















