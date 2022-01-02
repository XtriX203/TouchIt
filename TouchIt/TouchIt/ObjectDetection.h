#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <queue>

#define WHITE cv::Vec3b(255,255,255)
#define BLACK cv::Vec3b(0,0,0)

 
class ObjectDetection
{
private:
	cv::Mat bin;	//the binarized image
	//vector that stores the extreme top,bottom,left and right values of the object
	cv::Mat edge;
	cv::Mat AlignmentEdge;
	cv::Point FindFirstWhite(cv::Mat binImg);
	cv::Rect findBorder(std::queue<cv::Point> &queue);
	void checkBordersAround(cv::Point p, std::queue<cv::Point> &queue);
	bool isBorder(cv::Point p);
	bool isWhite(cv::Vec3b p, cv::Mat frame);
	void resetMaxHolder();
public:
	ObjectDetection(cv::Mat bin);
	cv::Mat Detect();
	
};

