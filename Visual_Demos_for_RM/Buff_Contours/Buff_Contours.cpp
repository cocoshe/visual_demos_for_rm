#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*	���demo�����۲������Ҷ������ֵ��֮�������
*	�۲�����֮��Ĵ�����ϵ
*	���������������ͼƬ�ı߿�������һ����������
*/


int main() {
	Mat binary, gray;
	Mat src = imread("buff.jpg");
	int width = src.cols;
	int height = src.rows;
	imshow("src", src);
//	src = src(Rect(15, 15, width - 30, height - 30));//ȡROI
//	imshow("src", src);
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 40, 255, THRESH_BINARY);

	imshow("binary", binary);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);//���ṹ���棬�������е�������
	
	for (int i = 0; i < contours.size(); i ++) {
		drawContours(src, contours, i, Scalar(0, 0, 255), 2);
	}
	imshow("dst", src);

	cout << endl;
	cout << "contours:" << contours.size() << endl;//���Ϊ5

	waitKey(0);
	return 0;
}