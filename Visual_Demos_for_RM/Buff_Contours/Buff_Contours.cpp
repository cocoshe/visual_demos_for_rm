#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*	这个demo用来观察体会扇叶经过二值化之后的轮廓
*	观察轮廓之间的从属关系
*	发现最外面的整个图片的边框总是算一个轮廓？？
*/


int main() {
	Mat binary, gray;
	Mat src = imread("buff.jpg");
	int width = src.cols;
	int height = src.rows;
	imshow("src", src);
//	src = src(Rect(15, 15, width - 30, height - 30));//取ROI
//	imshow("src", src);
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 40, 255, THRESH_BINARY);

	imshow("binary", binary);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);//树结构储存，储存所有的轮廓点
	
	for (int i = 0; i < contours.size(); i ++) {
		drawContours(src, contours, i, Scalar(0, 0, 255), 2);
	}
	imshow("dst", src);

	cout << endl;
	cout << "contours:" << contours.size() << endl;//结果为5

	waitKey(0);
	return 0;
}