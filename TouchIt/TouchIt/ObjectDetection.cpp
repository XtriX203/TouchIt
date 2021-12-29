#include "ObjectDetection.h"

std::vector<int> T_B_L_R = { -1,-1,-1,-1 };


bool ObjectDetection::isWhite(cv::Vec3b p, cv::Mat frame)
{
	if (p == WHITE) {
		return true;
	}
	return false;
}



//check if the pixel is an extreme pixel an updates if so
void checkMax(cv::Point p)
{
	//set the initial values of the object extreme borders to the first point
	if (T_B_L_R[0] == -1)
	{
		T_B_L_R[0] = p.x;
		T_B_L_R[1] = p.x;
		T_B_L_R[2] = p.y;
		T_B_L_R[3] = p.y;
	}
	else
	{
		//if the pixel is topper then the stored top (in the frame the order of the lines starts from the top)
		if (T_B_L_R[0] > p.x)
		{
			T_B_L_R[0] = p.x;
		}
		//pixel is lower then bottom
		if (T_B_L_R[1] < p.x)
		{
			T_B_L_R[1] = p.x;
		}
		//pixel is extreme left
		if (T_B_L_R[2] > p.y)
		{
			T_B_L_R[2] = p.y;
		}
		//pixel is extreme right
		if (T_B_L_R[3] < p.y)
		{
			T_B_L_R[3] = p.y;
		}
	}
}

//constractor
ObjectDetection::ObjectDetection(cv::Mat binImg)
{
	this->bin = binImg;
	this->edge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	this->AlignmentEdge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	for (int i = 0; i < edge.rows; i++)
	{
		for (int j = 0; j < edge.cols; j++)
		{
			edge.at<cv::Vec3b>(i, j) = BLACK;
			AlignmentEdge.at<cv::Vec3b>(i, j) = BLACK;
		}
	}
}

//findes all borders and colors them (recursive)
void ObjectDetection::findBorder(std::queue<cv::Point> &queue)
{
	//check if point is white on mat
	//checkMat(&vec, edge)
	cv::Point curr;
	while (!queue.empty())
	{
		curr = queue.front();
		queue.pop();
		
		edge.at<cv::Vec3b>(curr.x, curr.y) = WHITE; //some pixels are not colored and its make an infinite loop 
		//AlignmentEdge.at<cv::Vec3b>(vec[i].x, vec[i].y) = WHITE;//the visual edge detection
		checkMax(curr);
		checkBordersAround(curr,queue);
	}
	cv::Rect rect = cv::Rect(cv::Point(T_B_L_R[0], T_B_L_R[3]), cv::Point(T_B_L_R[1], T_B_L_R[2]));
	cv::rectangle(edge, rect, cv::Scalar(), 3);
}

//checks for border points near the current point
void ObjectDetection::checkBordersAround(cv::Point p, std::queue<cv::Point> &ret)
{
	
	bool upRight = true, downRight = true, upLeft = true, downLeft = true;

	//right
	if (p.y + 1 < edge.cols)
	{
		cv::Point toCheck1 = cv::Point(p.x, p.y + 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck1.x, toCheck1.y), edge))
			if (isBorder(toCheck1))
				ret.push(toCheck1);
	}	
	else
	{
		upRight = false;
		downRight = false;
	}
	//left
	if (p.y - 1 > 0)
	{
		cv::Point toCheck2 = cv::Point(p.x, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck2.x, toCheck2.y), edge))
			if (isBorder(toCheck2))
				ret.push(toCheck2);
	}
	else
	{
		upLeft = false;
		downLeft = false;
	}
	//down
	if (p.x + 1 < edge.rows)
	{
		cv::Point toCheck3 = cv::Point(p.x + 1, p.y);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck3.x, toCheck3.y), edge))
			if (isBorder(toCheck3))
				ret.push(toCheck3);
	}
	else
	{
		downRight = false;
		downLeft = false;
	}
	//up
	if (p.x - 1 > 0)
	{
		cv::Point toCheck4 = cv::Point(p.x-1, p.y);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck4.x, toCheck4.y), edge))
			if (isBorder(toCheck4))
				ret.push(toCheck4);
	}
	else
	{
		upLeft = false;
		upRight = false;
	}

	if (downRight)
	{
		cv::Point toCheck5 = cv::Point(p.x + 1, p.y + 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck5.x, toCheck5.y), edge))
			if (isBorder(toCheck5))
				ret.push(toCheck5);
	}
	if (downLeft)
	{
		cv::Point toCheck6 = cv::Point(p.x + 1, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck6.x, toCheck6.y), edge))
			if (isBorder(toCheck6))
				ret.push(toCheck6);
	}
	if (upRight)
	{
		cv::Point toCheck7 = cv::Point(p.x-1, p.y + 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck7.x, toCheck7.y), edge))
			if (isBorder(toCheck7))
				ret.push(toCheck7);
	}
	if (upLeft)
	{
		cv::Point toCheck8 = cv::Point(p.x - 1, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck8.x, toCheck8.y), edge))
			if (isBorder(toCheck8))
				ret.push(toCheck8);
	}

}

///check if the given point is a border
bool ObjectDetection::isBorder(cv::Point p)
{
	int whiteCount = 0, blackCount = 0;
	
	//check if the current point is part of the object (is white)
	if (!isWhite(bin.at<cv::Vec3b>(p.x, p.y), bin))
	{
		return false;
	}
	
	//right
	if (p.y + 1 < bin.cols)
		isWhite(bin.at<cv::Vec3b>(p.x, p.y + 1), bin) ? whiteCount++ : blackCount++;
	
	//left
	if (p.y - 1 > 0)
		isWhite(bin.at<cv::Vec3b>(p.x, p.y-1), bin) ? whiteCount++ : blackCount++;

	//down
	if (p.x + 1 < bin.rows)
		isWhite(bin.at<cv::Vec3b>(p.x + 1, p.y), bin) ? whiteCount++ : blackCount++;

	//up
	if (p.x - 1 > 0)
		isWhite(bin.at<cv::Vec3b>(p.x - 1, p.y), bin) ? whiteCount++ : blackCount++;

	if (whiteCount > 0 && blackCount > 0)
		return true;
	return false;
}



cv::Mat ObjectDetection::Detect()
{	
	cv::Point pixel = FindFirstWhite(this->bin);
 	std::queue<cv::Point> vec;
	vec.push(pixel);
	findBorder(vec);
	return this->edge;
}

cv::Point ObjectDetection::FindFirstWhite(cv::Mat bin)
{
	for (int i = 0; i < bin.rows; ++i)//its the Y(down)
	{
		for (int j = 0; j < bin.cols; ++j)//its the X(right)
		{
			cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
			if (pixel == WHITE)
			{
				return cv::Point(i , j); //test;
			}
		}
	}
	return cv::Point(0, 0);
}
