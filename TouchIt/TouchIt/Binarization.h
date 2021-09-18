#pragma once
#include <opencv2/imgproc.hpp>

class Binarization
{
public:
	static cv::Mat binarize(cv::Mat frame, int pointX, int pointY, int pointLen);	//main binarization func
	static cv::Mat convertToHSV(cv::Mat frame);
	static cv::Vec3b findLow(cv::Mat hsv, int pointX, int pointY, int pointLen);	//might be a way to pass a matrix of the needed square instead of the whole frame
	static cv::Vec3b findHigh(cv::Mat hsv, int pointX, int pointY, int pointLen);	//might be a way to pass a matrix of the needed square instead of the whole frame
	static cv::Mat mask(cv::Mat hsv, cv::Vec3b low, cv::Vec3b high);
};