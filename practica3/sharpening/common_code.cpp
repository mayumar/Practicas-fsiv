#include <iostream>
#include "common_code.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;

    // TODO 
    // Remember: 6*sigma is approx 99,73% of the distribution.



    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_extend_image(const cv::Mat& img, const cv::Size& new_size, int ext_type)
{
    CV_Assert(img.rows<new_size.height);
    CV_Assert(img.cols<new_size.width);
    cv::Mat out;
    //TODO
    //Hint: use cv::copyMakeBorder()

    //
    CV_Assert(out.type()==img.type());
    CV_Assert(out.rows == new_size.height);
    CV_Assert(out.cols == new_size.width);
    return out;
}

cv::Mat
fsiv_create_sharpening_filter(const int filter_type, int r1, int r2)
{
    CV_Assert(0<=filter_type && filter_type<=2);
    CV_Assert(0<r1 && r1<r2);
    cv::Mat filter;
    //TODO
    // Remember DoG = G[r2]-G[r1].
    // Hint: use fsiv_extend_image() to extent G[r1].
    // Remember: Use -Laplacian + [1] as filter.


    //
    CV_Assert(filter.type()==CV_32FC1);
    CV_Assert((filter_type == 2) || (filter.rows==3 && filter.cols==3) );
    CV_Assert((filter_type != 2) || (filter.rows==(2*r2+1) &&
                                     filter.cols==(2*r2+1)));
    return filter;
}



cv::Mat
fsiv_image_sharpening(const cv::Mat& in, int filter_type, bool only_luma,
                      int r1, int r2, bool circular)
{
    CV_Assert(in.depth()==CV_8U);
    CV_Assert(0<r1 && r1<r2);
    CV_Assert(0<=filter_type && filter_type<=2);
    cv::Mat out;
    
    // TODO
    // Hint: use cv::filter2D using cv::BORDER_ISOLATED.
    // Remember: if circular, first the input image must be circular extended,
    //  and then clip the result.


    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.size()==in.size());
    return out;
}
