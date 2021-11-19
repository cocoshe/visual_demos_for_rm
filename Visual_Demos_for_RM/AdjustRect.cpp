#include "opencv.hpp"
#include "iostream"

#define WIDTH_GREATER_THAN_HEIGHT 0
#define ANGLE_TO_UP 1

using namespace std;
using namespace cv;

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

void detect(Mat src) {
	cv::Mat binBrightImg, tempBinary;
	Mat _grayImg;
	cvtColor(src, _grayImg, COLOR_BGR2GRAY, 1);
	inRange(_grayImg, 100, 256, binBrightImg);
	//cv::threshold(_grayImg, binBrightImg, _param.brightness_threshold, 255, cv::THRESH_BINARY);
	// 红蓝通道相减
	std::vector<cv::Mat> splited;

	split(src, splited);
	subtract(splited[2], splited[0], tempBinary);//蓝-红
	threshold(tempBinary, tempBinary, 40, 255, cv::THRESH_BINARY);

	binBrightImg = tempBinary & binBrightImg;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	dilate(binBrightImg, binBrightImg, element);
	dilate(binBrightImg, binBrightImg, element);
	imshow("brightness_binary", binBrightImg);

	vector<vector<Point>> lightContours;
	cv::findContours(binBrightImg.clone(), lightContours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	// cv::findContours(binBrightImg.clone(), lightContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (const auto& contour : lightContours)
	{
		cout << contour.size() << endl;				//这里是防止contour的点太少了会报错(当然可以限制area的size，效果更好哈！)
		if (contour.size() < 5) continue;
		RotatedRect lightRec = fitEllipse(contour);	
		adjustRec(lightRec, ANGLE_TO_UP);			//事实上，这里的height并不一定比width大！！
//		adjustRec(lightRec, WIDTH_GREATER_THAN_HEIGHT);
		cout << "RotatedRect:height = " << lightRec.size.height << "   width = " << lightRec.size.width << "   angle = " << lightRec.angle << endl;
		Point2f p[4];
		lightRec.points(p);
		for (int i = 0; i < 4; i++) {
			line(src, p[i], p[(i + 1) % 4], Scalar(0, 0, 255));
		}
	}

}

int main() {
	VideoCapture video(0);
	Mat frame;
	while (1) {
		video >> frame;
		detect(frame);
		imshow("video", frame);
		waitKey(100);
	}

	return 0;
}