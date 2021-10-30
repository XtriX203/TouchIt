#include "Background_estimation.h"

/// <summary>
/// get frame of avarege colors from the queue of frames
/// </summary>
/// <param name="src">the queue of frames</param>
/// <param name="height">the height of all the frames</param>
/// <param name="width">the width of all the frames</param>
cv::Mat Background_estimation::GetAverageBG(cv::Mat avg, cv::Mat src, int height, int width, int mul)
{
	//set the frame we will return
	cv::Mat ret = cv::Mat(height, width, CV_8UC3);

	std::deque <cv::Mat> ::iterator it;

	//iterate all pixels in each frame
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int R_sum = 0, G_sum = 0, B_sum = 0;
			cv::Vec3b pixel = avg.at<cv::Vec3b>(i, j);
			cv::Vec3b curr = src.at<cv::Vec3b>(i, j);
			//set the colors
			R_sum = curr[0] + pixel[0] * mul;
			G_sum = curr[1] + pixel[1] * mul;
			B_sum = curr[2] + pixel[2] * mul;
			mul++;
			ret.at<cv::Vec3b>(i, j) = cv::Vec3b(R_sum / mul, G_sum / mul, B_sum / mul);
		}
	}
	return ret;
}

cv::Mat Background_estimation::MaskBin(cv::Mat avg, cv::Mat frame, int height, int width)
{
	cv::Mat ret = cv::Mat(height, width, CV_8UC3);
	cv::Vec3b pixel1, pixel2;
	cv::Vec3b curr;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pixel1 = avg.at<cv::Vec3b>(i, j);
			pixel2 = avg.at<cv::Vec3b>(i, j);
			for (int k = 0; k < 3; k++)
			{
				if (pixel1[k] <= 235)
					pixel1[k] += 20;
				else
					pixel1[k] = 255;

				if (pixel2[k] >= 20)
					pixel2[k] -= 20;
				else
					pixel2[k] = 0;
			}
			curr = frame.at<cv::Vec3b>(i, j);
			if (curr[0] < pixel1[0] && curr[0] > pixel2[0] &&
				curr[1] < pixel1[1] && curr[1] > pixel2[1] &&
				curr[2] < pixel1[2] && curr[2] > pixel2[2])
			{
				ret.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
			}
			else
			{
				ret.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
			}
		}
	}

	return ret;
}
