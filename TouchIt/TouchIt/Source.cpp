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

void Erosion(int, void*);
void Dilation(int, void*);
void liveCapture();
void loadImage();
void checkStop();

void binarization();
void edges();

bool stop = false;
bool once1 = false;
bool once2 = false;
int erosion_elem = 2;
int erosion_size = 3;
int dilation_elem = 2;
int dilation_size = 3;
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
	if ( frame.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
		std::cin.get(); //wait for any key press
		return;
	}

	ObjectDetection ob = ObjectDetection(frame);
	cv::imshow("Edge Lines", ob.Detect());
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

		if (!once2)
		{
			once2 = false;
			std::thread binarize(&binarization);
			binarize.detach();
		}
		
		if (stop && !once1)
		{
			once1 = true;
			std::thread edge(&edges);
			edge.detach();
		}
		
		//edgesMtx.lock();
		cv::Mat binCopy = bin;
		//edgesMtx.unlock();
		cv::imshow("original", frame);	//show the RGB frame
		cv::imshow("binarization", binCopy);
		cv::waitKey(1);
	}
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

void binarization()
{
	cv::Mat avgCopy, frameCopy, copyToBin;
	cv::Vec3b handColorCopy;
	while (true)
	{
		//binarizationMtx.lock();
		avgCopy = avg;
		frameCopy = frame;
		handColorCopy = handColor;
		//binarizationMtx.unlock();
		copyToBin = Background_estimation::MaskBin(avgCopy, frameCopy, handColorCopy, frameCopy.rows, frameCopy.cols);
		//edgesMtx.lock();
		bin = copyToBin;
		Erosion(0, 0);
		Dilation(0, 0);
		//edgesMtx.unlock();
	}
}

void edges()
{
	cv::Mat binCopy;
	while (true)
	{
		//edgesMtx.lock();
		binCopy = bin;
		//edgesMtx.unlock();
		ObjectDetection ob_detect = ObjectDetection(binCopy);

		cv::Mat edge = ob_detect.Detect();
		cv::imshow("Edge", edge);
		cv::waitKey(1);
	}
}
