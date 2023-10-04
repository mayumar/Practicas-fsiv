#include "common_code.hpp"

cv::Mat
fsiv_convert_image_byte_to_float(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.depth()==CV_8U);
    //TODO

    //
    CV_Assert(out.rows==img.rows && out.cols==img.cols);
    CV_Assert(out.depth()==CV_32F);
    CV_Assert(img.channels()==out.channels());
    return out;
}

cv::Mat
fsiv_convert_image_float_to_byte(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.depth()==CV_32F);
    //TODO

    //
    CV_Assert(out.rows==img.rows && out.cols==img.cols);
    CV_Assert(out.depth()==CV_8U);
    CV_Assert(img.channels()==out.channels());
    return out;
}

cv::Mat
fsiv_convert_bgr_to_hsv(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.channels()==3);
    //TODO

    //
    CV_Assert(out.channels()==3);
    return out;
}

cv::Mat
fsiv_convert_hsv_to_bgr(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.channels()==3);
    //TODO

    //
    CV_Assert(out.channels()==3);
    return out;
}

cv::Mat
fsiv_cbg_process (const cv::Mat & in, cv::Mat& out,
             double contrast, double brightness, double gamma,
             bool only_luma)
{
    CV_Assert(in.depth()==CV_8U);
    //TODO
    //Recuerda: es recomendable trabajar en flotante [0,1]
    //Después deshacer el cambio a byte [0,255]
    //Recuerda: usa código vectorizado.


    //
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    CV_Assert(out.depth()==CV_8U);
    CV_Assert(out.channels()==in.channels());
    return out;
}
