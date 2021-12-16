#pragma once
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <thread>
#include <conio.h>

class ColorModule
{
public:
	static cv::Vec3b colorManager();
	static cv::Vec3b findLow(cv::Mat frame, int pointX, int pointY, int pointLen);
	static cv::Vec3b findHigh(cv::Mat frame, int pointX, int pointY, int pointLen);
	static cv::Vec3b findAvg(cv::Vec3b high, cv::Vec3b low);

};