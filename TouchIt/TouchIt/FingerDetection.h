#pragma once
#include<vector>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#define WHITE cv::Vec3b(255,255,255)
class FingerDetection
{
private:
	cv::Mat _hand;
	std::vector<cv::Point> _fingers;
	cv::Point findMax(std::vector<cv::Point> one, std::vector<cv::Point> two);
	void colorFingers();
	cv::Point findNextWhite(cv::Point p, cv::Point prev);

public:
	FingerDetection(cv::Mat hand);
	std::vector<cv::Point> findFingers();
	std::vector<cv::Point> getFingers();
	cv::Point findTopFingerTip();
	cv::Mat getHand();
};

