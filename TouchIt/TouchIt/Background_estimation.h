#pragma once
#include <deque>
#include <opencv2/imgproc.hpp>

class Background_estimation
{
public:
	static cv::Mat GetAverageBG(cv::Mat avg, cv::Mat src, int height, int width, int mul);
	static cv::Mat MaskBin(cv::Mat avg, cv::Mat frame, int height, int width);
};