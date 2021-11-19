#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*测试一下仿射变换，用于矫正灯条
* getPerspectiveTransform和 warpPerspective 两个函数密切相关
* 常常同时出现，前者算出的是后者的一个参数
*/

int main() {
	VideoCapture video(0);
	Mat frame;
	Point2f src[] = { {0.0, 100.0}, {205.0, 100.0}, {200.0, 200.0}, {100.0, 200.0}};
	Point2f dst[] = { {100.0, 100.0}, {200.0, 100.0}, {200.0, 200.0}, {100.0, 200.0} };
	while (1) {
		video >> frame;
		float h = frame.rows;
		float w = frame.cols;
		Mat matrix = getPerspectiveTransform(src, dst);		//返回的是仿射变换用的矩阵。
															//意思是传入几个点，还有仿射变换后的目标点
															//它给你算出变换的矩阵
		warpPerspective(frame, frame, matrix, Size(w, h));	//利用上面算出的矩阵来对图片进行仿射变换
		imshow("src", frame);
		waitKey(10);
	}

	return 0;
}