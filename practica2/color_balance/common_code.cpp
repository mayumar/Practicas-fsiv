#include "common_code.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <opencv2/highgui.hpp>

cv::Mat fsiv_color_rescaling(const cv::Mat& in, const cv::Scalar& from, const cv::Scalar& to)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO
    //Cuidado con dividir por cero.
    //Evita los bucles.

    cv::Scalar conver;
    cv::divide(to, from, conver);
    cv::multiply(in, conver, out);

    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_wp_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO
    //Sugerencia: utiliza el espacio de color GRAY para
    //saber la ilumimancia de un pixel.

    cv::Mat inGray;
    cv::Point maxIdx;
    double max;
    cv::cvtColor(in, inGray, cv::COLOR_BGR2GRAY);

    cv::minMaxLoc(inGray, nullptr, &max, nullptr, &maxIdx);

    auto max3 = in.at<cv::Vec3b>(maxIdx);
    
    cv::Scalar white(255, 255, 255);

    out = fsiv_color_rescaling(in, max3, white);

    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_gw_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO

    auto mean = cv::mean(in);
    
    cv::Scalar gray(128, 128, 128);

    out = fsiv_color_rescaling(in, mean, gray);

    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_color_balance(cv::Mat const& in, float p)
{
    CV_Assert(in.type()==CV_8UC3);
    CV_Assert(0.0f<p && p<100.0f);
    cv::Mat out;
    //TODO
    //Sugerencia: utiliza el espacio de color GRAY para
    //saber la ilumimancia de un pixel.
    
    cv::Mat inGray;
    cv::cvtColor(in, inGray, cv::COLOR_BGR2GRAY);

    cv::Mat hist;
    int hist_size = 256;
    float channel[] = {0, 256};
    const float * ranges[] = {channel};
    cv::calcHist(&inGray, 1, 0, cv::Mat(), hist, 1, &hist_size, ranges);
    cv::normalize(hist, hist, 1.0, 0.0, cv::NORM_L1);

    int i = 0;
    float percent = 1.0 - (p/100.0), accum = 0.0;
    while(i < hist.rows && percent >= accum){
        accum += hist.at<float>(i);
        i++;
    }

    double max;
    cv::minMaxLoc(inGray, nullptr, &max);

    cv::Mat mask;
    cv::inRange(inGray, i, max, mask);

    auto mean = cv::mean(in, mask);
    cv::Scalar white(255, 255, 255);

    out = fsiv_color_rescaling(in, mean, white);


    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}
