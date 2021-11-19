/*
	����RotatedRect�еĽǶ�̽�����ó����½��ۣ�
	1.fitEllipes�������ص�RotatedRect��һ����:
		height > width;
		0 <= angle <= 180��;
		angleָwidth��x��ļн�(����Զ����x���·�����Ϊ��)
	2.adjustRec�Ծ��ν��н�������Ϊ�������ƽ�е�װ�װ壬����Ϊ����ƫor��ƫһ�������кܴ�ĽǶȲ�
	����ANGLE_TO_UP����45�ȸ�������width��heigh���Ӷ�ʧЧ��
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


vector<Point2f> P_FOR_ALL = { {100, 100}, {200, 150}, {300, 300}, {350, 350}, {250, 350} };	//ȫ�ֵĵ�
Point2f Center = {300, 300};	//��ת����


int main() {
	string trackBarName = "�Ƕȣ�";
	int posTrackBar = 0;	//TrackBar��ֵ
	int maxValue = 360;		//TrackBar����


	Mat image1(500, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat image2(500, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat image2bk;	//����
	image2bk = image2.clone();

	vector<Point2f> p = { {100, 100}, {200, 150}, {300, 300}, {350, 350}, {250, 350}};

	Draw(image1, p);								//�Լ��һ�һ�������

	RotatedRect rec1 = DrawRotatedRect(image1, p);	//����Բ��ϵõ���RotatedRect

	imshow("origin", image1);
	cout << "\n\n\n" << endl;
	cout << "RotatedRect1��width��" << rec1.size.width << endl;
	cout << "RotatedRect1��height��" << rec1.size.height << endl;
	cout << "RotatedRect1��angle��" << rec1.angle << endl;

	//����һ�£��۲�RotatedRect
	for (int i = 0; i < 5; i++) {
		p[i].x = image1.cols - p[i].x;
		p[i].y += 20;
	}

	Draw(image2, p);								//�Լ��һ�һ�������

	RotatedRect rec2 = DrawRotatedRect(image2, p);	//����Բ��ϵõ���RotatedRect

	imshow("change_dir", image2);
	
	cout << "\n\n\n" << endl;
	cout << "RotatedRect2��width��" << rec2.size.width << endl;
	cout << "RotatedRect2��height��" << rec2.size.height << endl;
	cout << "RotatedRect2��angle��" << rec2.angle << endl;



	createTrackbar(trackBarName, "change_dir", &posTrackBar, maxValue, onChangeTrackBar, &image2bk);

	waitKey(0);
}


/* ���Ӹ����� */
void Draw(Mat& image, vector<Point2f>& p) {
	for (int i = 0; i < 5; i++) {
		line(image, p[i], p[(i + 1) % 5], Scalar(0, 0, 255));
	}
}

/*
*	����Բ��ϵľ���
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




/* �ص����� */
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
	cout << "RotatedRect��width��" << rec.size.width << endl;
	cout << "RotatedRect��height��" << rec.size.height << endl;
	cout << "RotatedRect��angle��" << rec.angle << endl;

	rec = adjustRec(rec, ANGLE_TO_UP);					
//	rec = adjustRec(rec, WIDTH_GREATER_THAN_HEIGHT);	//�����б�̶ȣ����ǽǶȿ������һ�����ţ�-89���ܺ�89��ͬһ��װ�װ�ĵ���

	cout << "\n" << endl;
	cout << "after adjustRec()" << endl;
	cout << "RotatedRect��width��" << rec.size.width << endl;
	cout << "RotatedRect��height��" << rec.size.height << endl;
	cout << "RotatedRect��angle��" << rec.angle << endl;

	namedWindow("change_dir", WINDOW_NORMAL);
	imshow("change_dir", dst);
}

/*
* input:
*	Center	��ת���ĵ�
*	Points	��ת���ĵ�&
*	angle	ת���Ƕ�
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
*	RM�е�adjustRec
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
