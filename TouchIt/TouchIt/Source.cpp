#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>
#include <conio.h>
#include "Binarization.h"

void checkStop();
bool stop = false;

int main()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cv::Mat f;

	cv::Vec3b low;
	cv::Vec3b high;


	std::thread a(checkStop);
	a.detach();
	while (true)
	{
		cap.read(frame);
		flip(frame, frame, 1);
		f = frame.clone();	//create a copy of the frame to draw the square on

		cv::Size size = frame.size();	//get frame sizes
		int ponitX = size.width / 2 + 115;
		int pointY = size.height / 2 - 55;
		int len = 70;

		//draw the square
		cv::Rect rect = cv::Rect(ponitX, pointY, len, len);
		rectangle(f, rect, cv::Scalar(), 3);

		//get a binarized image
		cv::Mat hsv = Binarization::convertToHSV(frame);

		if (!stop)
		{
			low = Binarization::findLow(hsv, ponitX, pointY, len);
			high = Binarization::findHigh(hsv, ponitX, pointY, len);
		}


		cv::Mat bin = Binarization::mask(hsv, low, high);

		cv::imshow("original", f);	//show the RGB frame
		cv::imshow("Biary", bin);	//show the binirized image
		cv::waitKey(1);
	}
	return 0;
}

void checkStop()
{
	_getch();
	stop = true;
}
