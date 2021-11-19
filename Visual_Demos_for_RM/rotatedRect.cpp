#include "opencv.hpp"

using namespace cv;


/*****************************
����RotatedRect��angle�����ʣ�
	1.˳ʱ��ת��Ϊ+
	2.��ʱ��ת��Ϊ-
	3.����angle�ķ�Χ(��Ȼ�ٷ��ǰ�angle�ķ�Χ����-90�㵽0���)
******************************/


int main() {
	Mat image(200, 200, CV_8UC3, Scalar(0));
	RotatedRect rect(Point2f(100, 100), Size2f(100, 50), 30);//���ĵ㣬width height��angle

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
















