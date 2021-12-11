#include "ObjectDetection.h"

bool isWhite(cv::Point p,cv::Mat bin)
{
    if ( bin.at<cv::Vec3b>(p.x,p.y)== WHITE) {
        return true;
    }
    return false;
}

bool Move(cv::Point p, cv::Mat bin)
{//do the same as Detect and return if pixel given is match the condition
    return false;
}

cv::Mat ObjectDetection::Detect(cv::Mat bin)
{
    cv::Mat ret  =cv::Mat(bin.rows, bin.cols, CV_8UC3);
    cv::Point pixel=FindFirstWhite(bin);
    cv::Point up;
    pixel.y != 0 ? up = cv::Point(pixel.x, pixel.y - 1) : up = pixel;
    cv::Point down;
    cv::Point right;
    cv::Point left;
    int BCount = 0, WCount = 0;
    bool flag = true;

    while (flag)
    {
        //if true set the up pixel as curr pixel and call the function
        if (Move(up, bin)) {
            
        }
        pixel.y != bin.cols ? down = cv::Point(pixel.x, pixel.y + 1): down=pixel;        
        pixel.x != 0? left = cv::Point(pixel.x - 1, pixel.y):left=pixel;       
        pixel.x != bin.rows ?right = cv::Point(pixel.x + 1, pixel.y ): right=pixel;
       

        isWhite(up, bin) ? WCount++ : BCount++;
        isWhite(down, bin) ? WCount++ : BCount++;
        isWhite(right, bin) ? WCount++ : BCount++;
        isWhite(left, bin) ? WCount++ : BCount++;

        if (WCount > 0 && BCount > 0)
        {
            ret.at<cv::Vec3b>(pixel.x, pixel.y) = WHITE;
        }
        else
        {
            ret.at<cv::Vec3b>(pixel.x, pixel.y) = BLACK;
        }
    }

    return ret;
}

cv::Point ObjectDetection::FindFirstWhite(cv::Mat bin)
{
    for (int i = 0; i < bin.rows; i++)
    {
        for (int j = 0; j < bin.cols; j++)
        {
            cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
            if (pixel == WHITE)
            {
                return cv::Point(i,j);
            }
        }
    }
    return cv::Point(0,0);
}
