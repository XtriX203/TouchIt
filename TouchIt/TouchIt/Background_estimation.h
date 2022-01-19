#pragma once
#include <deque>
#include <opencv2/imgproc.hpp>

class Background_estimation
{
public:
	static cv::Mat GetAverageBG(cv::Mat avg, cv::Mat src, int height, int width, int mul);
	static cv::Mat MaskBin(cv::Mat avg, cv::Mat frame, cv::Vec3b targetColor, int height, int width);
	static cv::Vec3b GetClosestColor(cv::Vec3b target, cv::Mat frame, int height, int width);
	static bool isInRange(cv::Vec3b target, cv::Vec3b curr, int maxDiff);
};