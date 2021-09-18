#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Binarization.h"

int main()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cv::Mat f;

	while (true)
	{
		cap.read(frame);
		flip(frame, frame, 1);
		f = frame.clone();	//create a copy of the frame to draw the square on

		cv::Size size = frame.size();	//get frame sizes

		//draw the square
		cv::Rect rect = cv::Rect(size.width / 2 + 115, size.height / 2 - 55, 70, 70);
		rectangle(f, rect, cv::Scalar(), 3);

		//get a binarized image
		cv::Mat bin = Binarization::binarize(frame, size.width / 2 + 115, size.height / 2 - 55, 70);

		cv::imshow("original", f);	//show the RGB frame
		cv::imshow("Biary", bin);	//show the binirized image
		cv::waitKey(1);
	}
	return 0;
}