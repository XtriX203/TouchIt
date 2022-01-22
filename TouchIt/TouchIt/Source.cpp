#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>
#include <conio.h>
#include <deque>
#include "ColorModule.h"
#include "Background_estimation.h"
#include "ObjectDetection.h"
#include <mutex>

std::mutex edgesMtx;
std::mutex binarizationMtx;

void open(int, void*);
void close(int, void*);
void liveCapture();
void loadImage();
void checkStop();

void binarization();
void edges();

bool stop = false;

int open_elem = 2;
int open_size = 2;
int close_elem = 2;
int close_size = 2;
int const max_elem = 2;
int const max_kernel_size = 21;
cv::Mat bin;

int main()
{


	std::cout << "choose run option:\n1)live capture\n2)load images\n";
	int choise;
	std::cin >> choise;

	switch (choise)
	{
	case 1:
		liveCapture();
		break;
	case 2:

		loadImage();
		break;
	default:
		break;
	}

	return 0;
}

void loadImage()
{
	std::string img;
	std::cout << "Enter path of current image:\n";
	std::cin >> img;

	cv::Mat frame = cv::imread(img);

	// Check for failure
	if (frame.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
		std::cin.get(); //wait for any key press
		return;
	}

	ObjectDetection ob = ObjectDetection(frame);
	cv::imshow("Edge Lines", ob.detect());
	cv::imshow("Hand", ob.getHand());
	cv::imshow("original", frame);	//show the RGB frame
	cv::waitKey(0);

}

cv::Mat avg;
cv::Mat frame;
cv::Vec3b handColor;
void liveCapture()
{
	handColor = ColorModule::colorManager();//getting the hand color(from the rectangle)

	cv::VideoCapture cap(0);
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
		//binarizationMtx.lock();
		cap.read(frame);
		flip(frame, frame, 1);
		cv::Mat frameCopy = frame;
		//binarizationMtx.unlock();

		//get a binarized image

		if (!stop)
		{
			//binarizationMtx.lock();
			avg = Background_estimation::GetAverageBG(avg, frame, frame.rows, frame.cols, mul);
			//binarizationMtx.unlock();
			cv::imshow("average", avg);	//show the RGB frame
		}



		binarization();
		if (stop)
		{
			edges();
		}
		//edgesMtx.lock();
		cv::Mat binCopy = bin;
		//edgesMtx.unlock();
		cv::imshow("original", frame);	//show the RGB frame
		cv::imshow("binarization", binCopy);
		cv::waitKey(1);
	}
}

void open(int, void*)
{
	int open_type = 0;
	if (open_elem == 0) {
		open_type = cv::MORPH_RECT;
	}
	else if (open_elem == 1) {
		open_type = cv::MORPH_CROSS;
	}
	else if (open_elem == 2) {
		open_type = cv::MORPH_ELLIPSE;
	}
	//cv::Mat element = cv::getStructuringElement(erosion_type,
	//	cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
	//	cv::Point(erosion_size, erosion_size));
	//cv::erode(bin, bin, element);
	cv::Mat element = cv::getStructuringElement(open_type, cv::Size(2 * open_size + 1, 2 * open_size + 1), cv::Point(open_size, open_size));
	cv::morphologyEx(bin, bin, cv::MORPH_OPEN, element);
}

void close(int, void*)
{
	int close_type = 0;
	if (close_elem == 0) {
		close_type = cv::MORPH_RECT;
	}
	else if (close_elem == 1) {
		close_type = cv::MORPH_CROSS;
	}
	else if (close_elem == 2) {
		close_type = cv::MORPH_ELLIPSE;
	}
	//cv::Mat element = cv::getStructuringElement(dilation_type,
	//	cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
	//	cv::Point(dilation_size, dilation_size));
	//dilate(bin, bin, element);
	cv::Mat element = cv::getStructuringElement(close_type, cv::Size(2 * close_size + 1, 2 * close_size + 1), cv::Point(close_size, close_size));
	cv::morphologyEx(bin, bin, cv::MORPH_CLOSE, element);
}

void checkStop()
{
	_getch();
	stop = true;
}

void binarization()
{
	cv::Mat avgCopy, frameCopy, copyToBin;
	cv::Vec3b handColorCopy;
	//while (true)
	//{
		//binarizationMtx.lock();
	avgCopy = avg;
	frameCopy = frame;
	handColorCopy = handColor;
	//binarizationMtx.unlock();
	copyToBin = Background_estimation::MaskBin(avgCopy, frameCopy, handColorCopy, frameCopy.rows, frameCopy.cols);
	//edgesMtx.lock();
	bin = copyToBin;
	open(0,0);
	close(0,0);
	//edgesMtx.unlock();
//}
}

void edges()
{
	cv::Mat binCopy;
	binCopy = bin;
	ObjectDetection ob_detect = ObjectDetection(binCopy);
	cv::Mat edge = ob_detect.detect();
	cv::Mat hand = ob_detect.getHand();
	cv::imshow("Hand", hand);
	cv::waitKey(1);

}
