#pragma once
#include <deque>
#include <opencv2/imgproc.hpp>

class Background_estimation
{
public:
	static cv::Mat GetAverageBG(std::deque<cv::Mat> src, int height, int width);

};