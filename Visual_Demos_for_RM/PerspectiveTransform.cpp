#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*����һ�·���任�����ڽ�������
* getPerspectiveTransform�� warpPerspective ���������������
* ����ͬʱ���֣�ǰ��������Ǻ��ߵ�һ������
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
		Mat matrix = getPerspectiveTransform(src, dst);		//���ص��Ƿ���任�õľ���
															//��˼�Ǵ��뼸���㣬���з���任���Ŀ���
															//����������任�ľ���
		warpPerspective(frame, frame, matrix, Size(w, h));	//������������ľ�������ͼƬ���з���任
		imshow("src", frame);
		waitKey(10);
	}

	return 0;
}