#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>
#include <conio.h>
#include <deque>
#include "Binarization.h"
#include "Background_estimation.h"

void Erosion(int, void*);
void Dilation(int, void*);

void checkStop();
bool stop = false;
int erosion_elem = 2;
int erosion_size = 3;
int dilation_elem = 2;
int dilation_size = 3;
int const max_elem = 2;
int const max_kernel_size = 21;
cv::Mat bin;

int main()
{
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cv::Mat avg;
	cv::Mat background;
	int mul = 1;

	cap.read(frame);
	flip(frame, frame, 1);
	avg = frame;
	bin = frame;

	std::thread a(checkStop);
	a.detach();

	while (true)
	{
		cap.read(frame);
		flip(frame, frame, 1);

		//get a binarized image

		if (!stop)
		{
			//background.pop_front();
			avg = Background_estimation::GetAverageBG(avg, frame, frame.rows, frame.cols, mul);
			//mul++;
			cv::imshow("average", avg);	//show the RGB frame
		}

		bin = Background_estimation::MaskBin(avg, frame, frame.rows, frame.cols);
		Erosion(0, 0);
		Dilation(0, 0);
		//cv::Mat bin = Binarization::mask(hsv, low, high);

		cv::imshow("original", frame);	//show the RGB frame


		cv::imshow("binarization", bin);
		cv::waitKey(1);
	}
	return 0;
}
	void Erosion(int, void*)
	{
		int erosion_type = 0;
		if (erosion_elem == 0) {
			erosion_type = cv::MORPH_RECT;
		}
		else if (erosion_elem == 1) {
			erosion_type = cv::MORPH_CROSS;
		}
		else if (erosion_elem == 2) {
			erosion_type = cv::MORPH_ELLIPSE;
		}
		cv::Mat element = cv::getStructuringElement(erosion_type,
			cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			cv::Point(erosion_size, erosion_size));
		cv::erode(bin, bin, element);
	}

	void Dilation(int, void*)
	{
		int dilation_type = 0;
		if (dilation_elem == 0) {
			dilation_type = cv::MORPH_RECT;
		}
		else if (dilation_elem == 1) {
			dilation_type = cv::MORPH_CROSS;
		}
		else if (dilation_elem == 2) {
			dilation_type = cv::MORPH_ELLIPSE;
		}
		cv::Mat element = cv::getStructuringElement(dilation_type,
			cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			cv::Point(dilation_size, dilation_size));
		dilate(bin, bin, element);
	}

void checkStop()
{
	_getch();
	stop = true;
}
