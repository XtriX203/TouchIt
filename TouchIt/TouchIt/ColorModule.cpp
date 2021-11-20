#include "ColorModule.h"
#include "Binarization.h"

//for RGB
#define RED 0
#define GREEN 1
#define BLUE 2

bool wait = false;

void waitStop()
{
	_getch();
	wait = true;
}


cv::Vec3b ColorModule::colorManager()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cv::Mat f;
	cv::Mat hsv;

	cv::Vec3b low;
	cv::Vec3b high;

	std::thread a(waitStop);
	a.detach();

	while (!wait)
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

		if (wait)
		{
			//hsv = Binarization::convertToHSV(frame);
			low = ColorModule::findLow(frame, ponitX, pointY, len);
			high = ColorModule::findHigh(frame, ponitX, pointY, len);
		}
		cv::imshow("original", f);	//show the RGB frame
		cv::waitKey(1);
	}
    return findAvg(high,low);
}

cv::Vec3b ColorModule::findLow(cv::Mat frame, int pointX, int pointY, int pointLen)
{
	cv::Vec3b low(255, 255, 255);	//initialize with the highest values. will be returned

	for (int i = pointX; i < pointX + pointLen; i++)
	{
		for (int j = pointY; j < pointY + pointLen; j++)
		{
			cv::Vec3b curr = frame.at<cv::Vec3b>(j, i);	//the current pixel

			if (curr[RED] < low[RED])	//check if the current RED is lower
			{
				low[RED] = curr[RED];
			}
			if (curr[GREEN] < low[GREEN])	//check if the current GREEN is lower
			{
				low[GREEN] = curr[GREEN];
			}
			if (curr[BLUE] < low[BLUE])	//check if the current BLUE is lower
			{
				low[BLUE] = curr[BLUE];
			}
		}
	}
	return low;
}

cv::Vec3b ColorModule::findHigh(cv::Mat frame, int pointX, int pointY, int pointLen)
{
	cv::Vec3b high(0, 0, 0);

	/*i will not remove the code yet, i thing more testing is needed*/
	for (int i = pointX; i < pointX + pointLen; i++)
	{
		for (int j = pointY; j < pointY + pointLen; j++)
		{
			cv::Vec3b curr = frame.at<cv::Vec3b>(j, i);

			if (curr[RED] > high[RED])
			{
				high[RED] = curr[RED];
			}
			if (curr[GREEN] > high[GREEN])
			{
				high[GREEN] = curr[GREEN];
			}
			if (curr[BLUE] > high[BLUE])
			{
				high[BLUE] = curr[BLUE];
			}
		}
	}
	return high;
}

cv::Vec3b ColorModule::findAvg(cv::Vec3b high, cv::Vec3b low)
{
	
    return cv::Vec3b((high[RED]+low[RED])/2, (high[GREEN] + low[GREEN]) / 2, (high[BLUE] + low[BLUE]) / 2);
}
