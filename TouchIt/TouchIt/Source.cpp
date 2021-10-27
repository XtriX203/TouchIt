#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>
#include <conio.h>
#include <deque>
#include "Binarization.h"
#include "Background_estimation.h"

void checkStop();
bool stop = false;

int main()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cv::Mat avg;
	std::deque<cv::Mat > background;

	std::thread a(checkStop);
	a.detach();
	while (true)
	{
		cap.read(frame);
		flip(frame, frame, 1);

		//get a binarized image
		if (!stop)
		{
			if (background.size() == 10)
			{
				background.pop_front();
			}
			background.push_back(frame);
			avg = Background_estimation::GetAverageBG(background,frame.rows,frame.cols);
		}

		//cv::Mat bin = Binarization::mask(hsv, low, high);

		cv::imshow("original", frame);	//show the RGB frame
		cv::imshow("average", avg);	//show the RGB frame
		
		cv::waitKey(1);
	}
	return 0;
}

void checkStop()
{
	_getch();
	stop = true;
}
