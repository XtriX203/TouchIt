#include "FingerDetection.h"

#define GREEN cv::Scalar(0, 255, 0)
#define RED cv::Scalar(0, 0, 255)

FingerDetection::FingerDetection(cv::Mat hand): _hand(hand)
{
}

std::vector<cv::Point> FingerDetection::findFingers()
{
	cv::Mat test = _hand;	//rem

	cv::Point min;
	std::vector<cv::Point> temp;
	cv::Point top = findTopFingerTip();
	this->_fingers.push_back(top);
	cv::Point curr = top;
	cv::Point prev = cv::Point(top.x + 1, top.y);
	cv::Point prevHolder;
	int counter = 0;
	bool up = false, once = false;
	std::vector<cv::Point> one;
	std::vector<cv::Point> two;
	bool leftSide = false;
	if (top.x == -1) { return this->_fingers; }
	two.push_back(curr);
	test.at<cv::Vec3b>(curr.x, curr.y) = cv::Vec3b(0, 255, 255); //rem
	do{
		counter++;
		prevHolder = curr;
		curr = findNextWhite(curr, prev);
		if (curr.x == -1 && leftSide)
			break;
		else if (curr.x == -1)
		{
			//prevHolder = findNextWhite(top, cv::Point(-1, 1));
			curr = findNextWhite(top, cv::Point(-1, 1));
			leftSide = true;
			once = false;
			up = false;
			one.clear();
			two.clear();
			if (curr.x == -1)
				break;
		}
		test.at<cv::Vec3b>(curr.x, curr.y) = cv::Vec3b(0, 255, 255); //rem
		//cv::imshow("test", test);
		//cv::waitKey(1);
		prev = prevHolder;
		two.push_back(curr);

		if (counter == this->_hand.cols / 10 || curr == top)
		{
			if (two[0].x - two[two.size()-1].x < 0)
			{
				if (up)
				{
					this->_fingers.push_back(findMax(one, two));
					up = false;
					once = false;
				}
			}
			else
			{
				up = true;
				if (!once)
					temp.push_back(this->findMin(one, two));
				once = true;
			}
			one = two;
			two.clear();
			counter = 0;
		}
	}while (curr != top);
	colorFingers(this->_fingers, GREEN);
	colorFingers(temp, RED);
	//cv::imshow("test", test); //rem

	return this->_fingers;
}

cv::Point FingerDetection::findMax(std::vector<cv::Point> one, std::vector<cv::Point> two)
{
	cv::Point max2 = two[0];
	cv::Point max1;

	if (!one.empty())
		max1 = one[0];
	else
		max1 = cv::Point(-1, -1);

	for (int i = 1; i < two.size(); i++)
	{
		max2 = max2.x < two[i].x ? max2 : two[i];
	}
	for (int i = 1; i < one.size(); i++)
	{
		max1 = max1.x < one[i].x ? max1 : one[i];
	}

	return max1.x < max2.x ? max1 : max2;
}

cv::Point FingerDetection::findMin(std::vector<cv::Point> one, std::vector<cv::Point> two)
{
	cv::Point min2 = two[0];
	cv::Point min1;

	if (!one.empty())
		min1 = one[0];
	else
		min1 = cv::Point(this->_hand.rows, this->_hand.cols);

	for (int i = 1; i < two.size(); i++)
	{
		min2 = min2.x > two[i].x ? min2 : two[i];
	}
	for (int i = 1; i < one.size(); i++)
	{
		min1 = min1.x > one[i].x ? min1 : one[i];
	}

	return min1.x > min2.x ? min1 : min2;
}

void FingerDetection::colorFingers(std::vector<cv::Point> vec, cv::Scalar s)
{
	for (int i = 0; i < vec.size(); i++)
	{
		cv::Rect rect = cv::Rect(cv::Point(vec[i].y - 1 < 0 ? vec[i].y : vec[i].y - 1,
			vec[i].x - 1 < 0 ? vec[i].x : vec[i].x - 1),
			cv::Point(vec[i].y + 1 >= this->_hand.cols ? vec[i].y : vec[i].y + 1,
				vec[i].x + 1 >= this->_hand.rows ? vec[i].x : vec[i].x + 1));
		cv::rectangle(this->_hand, rect, s, 5);
	}
}

cv::Point FingerDetection::findNextWhite(cv::Point p, cv::Point prev)
{
	//right
	if (p.y + 1 < _hand.cols)
	{
		if (prev != cv::Point(p.x, p.y + 1) && this->_hand.at<cv::Vec3b>(p.x, p.y + 1) == WHITE)
		{
			return cv::Point(p.x, p.y+1);
		}
	}
	//down
	if (p.x + 1 < _hand.rows *3/4)
	{
		if (prev != cv::Point(p.x + 1, p.y) && this->_hand.at<cv::Vec3b>(p.x+1, p.y) == WHITE)
		{
			return cv::Point(p.x+1, p.y);
		}
	}
	//up
	if (p.x -1 >= 0)
	{
		if (prev != cv::Point(p.x - 1, p.y) && this->_hand.at<cv::Vec3b>(p.x - 1, p.y) == WHITE)
		{
			return cv::Point(p.x - 1, p.y);
		}
	}
	//left
	if (p.y - 1 >= 0)
	{
		if (prev != cv::Point(p.x, p.y - 1) && this->_hand.at<cv::Vec3b>(p.x , p.y - 1) == WHITE)
		{
			return cv::Point(p.x , p.y-1);
		}
	}
	//down right
	if (p.y + 1 < _hand.cols && p.x + 1 < _hand.rows *3/4)
	{
		if (prev != cv::Point(p.x, p.y + 1) && this->_hand.at<cv::Vec3b>(p.x + 1 , p.y + 1) == WHITE)
		{
			return cv::Point(p.x + 1, p.y + 1);
		}
	}
	//up right
	if (p.y + 1 < _hand.cols && p.x - 1 >= 0)
	{
		if (prev != cv::Point(p.x - 1, p.y + 1) && this->_hand.at<cv::Vec3b>(p.x - 1, p.y + 1) == WHITE)
		{
			return cv::Point(p.x - 1, p.y + 1);
		}
	}
	//down left
	if (p.y - 1 >= 0 && p.x + 1 < _hand.rows *3/4)
	{
		if (prev != cv::Point(p.x + 1, p.y - 1) && this->_hand.at<cv::Vec3b>(p.x + 1, p.y - 1) == WHITE)
		{
			return cv::Point(p.x + 1, p.y - 1);
		}
	}
	//up left
	if (p.y - 1 >= 0 && p.x - 1 >= 0)
	{
		if (prev != cv::Point(p.x - 1, p.y - 1) && this->_hand.at<cv::Vec3b>(p.x - 1, p.y - 1) == WHITE)
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

cv::Mat FingerDetection::getHand()
{
	return this->_hand;
}
