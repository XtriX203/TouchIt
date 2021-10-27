#include "Background_estimation.h"

/// <summary>
/// get frame of avarege colors from the queue of frames
/// </summary>
/// <param name="src">the queue of frames</param>
/// <param name="height">the height of all the frames</param>
/// <param name="width">the width of all the frames</param>
cv::Mat Background_estimation::GetAverageBG(std::deque<cv::Mat> src, int height, int width)
{
    //set the frame we will return
    cv::Mat ret=cv::Mat(height, width, CV_8UC3);
    int R_sum=0, G_sum=0, B_sum=0,count=1;

    std::deque <cv::Mat> ::iterator it;
    //iterate all the frames in the deque
    for (it = src.begin(); it != src.end(); ++it)
    {
        //iterate all pixels in each frame
        for (int i = 0; i < height ;i++) {
            for (int j = 0; j < width; j++) {
                cv::Vec3b Pixel;
                cv::Vec3b curr = (*it).at<cv::Vec3b>(i, j);
                //set the colors
                R_sum += curr[0];
                G_sum += curr[1];
                B_sum += curr[2];
                //create the final pixel
                Pixel = cv::Vec3b(R_sum/count, G_sum / count, B_sum / count);

                ret.at<cv::Vec3b>(i, j) = Pixel;
                R_sum = 0, B_sum = 0, G_sum = 0;
            }
        }
        count++;
    }
    return ret;
}
