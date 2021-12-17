#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#define WHITE cv::Vec3b(255,255,255)
#define BLACK cv::Vec3b(0,0,0)

 
class ObjectDetection
{
private:
	cv::Mat bin;	//the binarized image
	//vector that stores the extreme top,bottom,left and right values of the object
	cv::Mat edge;
public:
	ObjectDetection(cv::Mat bin);
	cv::Mat Detect(cv::Mat bin);
	cv::Point FindFirstWhite(cv::Mat binImg);
	void findBorder(std::vector<cv::Point2i> vec);
	std::vector<cv::Point2i> checkBordersAround(cv::Point2i p);
	bool isBorder(cv::Point2i p);
};

