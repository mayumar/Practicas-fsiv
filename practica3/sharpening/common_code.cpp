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

    std::vector<int> sizes = {2*r+1, 2*r+1};
    ret_v = cv::Mat(sizes, CV_32FC1);

    float sigma = (2*r+1)/6.0;
    for(int i = 0; i < 2*r+1; i++){
        for(int j = 0; j < 2*r+1; j++){
            float eq = (1.0/(sigma*sigma*2.0*M_PI)) * exp(-(pow(i-r,2)+pow(j-r,2))/(2.0*sigma*sigma));
            ret_v.at<float>(cv::Point(i,j)) = eq;
        }
    }

    ret_v /= cv::sum(ret_v);

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

    int rh = (new_size.height-img.rows)/2;
    int rw = (new_size.width-img.cols)/2;
    out = cv::Mat::zeros(new_size.height, new_size.width, img.type());

    if(ext_type == 1){
        img(cv::Rect(0,img.rows-rh,img.cols,rh)).copyTo(out(cv::Rect(rw, 0, img.cols,rh)));
        img(cv::Rect(0,0,img.cols,rh)).copyTo(out(cv::Rect(rw,out.rows-rh, img.cols,rh)));

        img(cv::Rect(img.cols-rw,0,rw,img.rows)).copyTo(out(cv::Rect(0,rh,rw,img.rows)));
        img(cv::Rect(0,0,rw,img.rows)).copyTo(out(cv::Rect(out.cols-rw,rh,rw,img.rows)));

        img(cv::Rect(img.cols-rw,img.rows-rh,rw,rh)).copyTo(out(cv::Rect(0,0,rw,rh)));
        img(cv::Rect(0,img.rows-rh,rw,rh)).copyTo(out(cv::Rect(out.cols-rw,0,rw,rh)));
        img(cv::Rect(img.cols-rw,0,rw,rh)).copyTo(out(cv::Rect(0,out.rows-rh,rw,rh)));
        img(cv::Rect(0,0,rw,rh)).copyTo(out(cv::Rect(out.cols-rw,out.rows-rh,rw,rh)));
    }

    img.copyTo(out(cv::Rect(rw, rh, img.cols, img.rows)));

    /*
        int r = (new_size.height-img.rows)/2;
        if(ext_type == 0)
            cv::copyMakeBorder(img, out, r, r, r, r, cv::BORDER_CONSTANT);
        else
            cv::copyMakeBorder(img, out, r, r, r, r, cv::BORDER_WRAP);
    */
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
    if(filter_type == 0){
        filter = cv::Mat::zeros(3, 3, CV_32FC1);
        filter.at<float>(1,0) = -1;
        filter.at<float>(0,1) = -1;
        filter.at<float>(1,1) = 5;
        filter.at<float>(1,2) = -1;
        filter.at<float>(2,1) = -1;
    }else if(filter_type == 1){
        filter = cv::Mat(3, 3, CV_32FC1, -1);
        filter.at<float>(1,1) = 9;
    }else{
        auto G1 = fsiv_create_gaussian_filter(r1);
        auto G2 = fsiv_create_gaussian_filter(r2);

        G1 = fsiv_extend_image(G1, G2.size(), 0);
        filter = G2-G1;
        filter = -filter;
        filter.at<float>(filter.rows/2, filter.cols/2)++;
    }

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
    auto filter = fsiv_create_sharpening_filter(filter_type, r1, r2);
    
    if(!only_luma){
        cv::Mat in_ext;
        in.copyTo(in_ext);

        in_ext = fsiv_extend_image(in, cv::Size(in.cols+(2*r1), in.rows+(2*r2)), circular);

        cv::Mat out_ext;
        cv::filter2D(in_ext, out_ext, -1, filter, cv::Point(-1,-1), (0.0), cv::BORDER_ISOLATED);
        out = out_ext(cv::Rect(r1, r2, in.cols, in.rows));
    }else{
        cv::Mat in_hsv;
        cv::cvtColor(in, in_hsv, cv::COLOR_BGR2HSV);
        std::vector<cv::Mat> channels;
        cv::split(in_hsv, channels);

        channels[2] = fsiv_extend_image(channels[2], cv::Size(in.cols+(2*r2), in.rows+(2*r2)), circular);

        cv::filter2D(channels[2], channels[2], -1, filter, cv::Point(-1,-1), (0.0), cv::BORDER_ISOLATED);
        channels[2] = channels[2](cv::Rect(r2, r2, in.cols, in.rows));

        cv::merge(channels, out);

        cv::cvtColor(out, out, cv::COLOR_HSV2BGR);
    }

    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.size()==in.size());
    return out;
}
