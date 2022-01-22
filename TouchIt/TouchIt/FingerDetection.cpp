#include "FingerDetection.h"

FingerDetection::FingerDetection(cv::Mat hand): _hand(hand)
{
}

std::vector<cv::Point> FingerDetection::findFingers()
{
	cv::Point top = findTopFingerTip();
	cv::Point curr;
	if (top.x == -1) { return; }
	do{
		
	}while (curr != top);

}

cv::Point FingerDetection::findNextWhite(cv::Point p)
{
	//right
	if (p.y + 1 < _hand.cols)
	{
		if (this->_hand.at<cv::Vec3b>(p.x, p.y + 1) == WHITE)
		{
			return cv::Point(p.x, p.y+1);
		}
	}
	//down
	if (p.x + 1 < _hand.rows)
	{
		if (this->_hand.at<cv::Vec3b>(p.x+1, p.y) == WHITE)
		{
			return cv::Point(p.x+1, p.y);
		}
	}
	//up
	if (p.x -1 >= 0)
	{
		if (this->_hand.at<cv::Vec3b>(p.x - 1, p.y) == WHITE)
		{
			return cv::Point(p.x - 1, p.y);
		}
	}
	//left
	if (p.y - 1 >= 0)
	{
		if (this->_hand.at<cv::Vec3b>(p.x , p.y - 1) == WHITE)
		{
			return cv::Point(p.x , p.y-1);
		}
	}
	//down right
	if (p.y + 1 <= _hand.cols && p.x + 1 <= _hand.rows)
	{
		if (this->_hand.at<cv::Vec3b>(p.x + 1 , p.y + 1) == WHITE)
		{
			return cv::Point(p.x + 1, p.y + 1);
		}
	}
	//up right
	if (p.y + 1 <= _hand.cols && p.x - 1 >= 0)
	{
		if (this->_hand.at<cv::Vec3b>(p.x - 1, p.y + 1) == WHITE)
		{
			return cv::Point(p.x - 1, p.y + 1);
		}
	}
	//down left
	if (p.y - 1 >= 0 && p.x + 1 <= _hand.rows)
	{
		if (this->_hand.at<cv::Vec3b>(p.x + 1, p.y - 1) == WHITE)
		{
			return cv::Point(p.x + 1, p.y - 1);
		}
	}
	//up left
	if (p.y - 1 >= 0 && p.x - 1 >= 0)
	{
		if (this->_hand.at<cv::Vec3b>(p.x - 1, p.y - 1) == WHITE)
		{
			return cv::Point(p.x - 1, p.y - 1);
		}
	}
	return cv::Point(-1, -1);
}

std::vector<cv::Point> FingerDetection::getFingers()
{
	return std::vector<cv::Point>();
}


cv::Point FingerDetection::findTopFingerTip()
{
	for (int i = 0; i < this->_hand.cols; i++)
	{
		if(this->_hand.at<cv::Vec3b>(0, i)==WHITE)
		{	
			return cv::Point(0, i);
		}
	}
	return cv::Point(-1, -1);
}
