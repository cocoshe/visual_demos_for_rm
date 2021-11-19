#include "opencv.hpp"
#include <iostream>

using namespace cv;

/****************
����RotatedRect����ͨRect����
ǰ�߿�����ת�����߲�����ת��
���⣬Rectangle����ֱ��������Rect���÷���line������ֱ��һ��

fillConvexPoly������
���ܣ����ö��������һ�������
������ѡ���壻�������ζ���vector����ɫ
*****************/


int main() {
	Mat image(200, 200, CV_8UC3, Scalar(0));
	RotatedRect r_rect(Point2f(100, 100), Size2f(100, 50), 60);

	Point2f p[4];
	r_rect.points(p);

	//��RotatedRect
	for (int i = 0; i < 4; i++) {
		line(image, p[i], p[(i + 1) % 4], Scalar(0, 0, 255));
	}

	//ȡ��Ӿ��Σ���rectangle���������Rect
	Rect rect = r_rect.boundingRect();
	rectangle(image, rect, Scalar(0, 255 ,0));

	Mat lightMask = Mat::zeros(rect.size(), CV_8UC1);

	r_rect.points(p);
	std::vector<Point> RectPoints;

	/*
		�����ѭ�����൱�������RotatedRect�����Rect�����Ͻǵ����λ��
		
		��ʵ���Ե�������ȡ��һ�����Rect��ROI
		Ȼ����ȫ�ڵ�Mask����
		���µõ���RotatedRect���Rect�Ķ�����ж������䣬��ɫΪ255��ɫ
	*/
	for (int i = 0; i < 4; i++)
	{
		RectPoints.emplace_back(Point(p[i].x - rect.tl().x,
			p[i].y - rect.tl().y));
	}
	fillConvexPoly(lightMask, RectPoints, 255);		//��ȫ��Mask���滭����ɫ��rotated����
	imshow("fill", lightMask);						//��ʾ�Լ�����Mask
	imshow("r_rect", image);						//��ʾԭ����Ԥ�������ʵͼ��
	waitKey(0);
}







