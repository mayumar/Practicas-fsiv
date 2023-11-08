#include "common_code.hpp"

cv::Mat
fsiv_create_box_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;
    // TODO
    // Hint: use the constructor of cv::Mat to set the proper initial value.
    std::vector<int> sizes = {2*r+1, 2*r+1};
    int nele = (2*r+1) * (2*r+1);
    ret_v = cv::Mat(sizes, CV_32FC1, 1.0/(nele));
    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;
    // TODO
    // Remember: 6*sigma is approx 99,73% of the distribution.
    // Remember: scale the filter coefficients to sum 1.
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
fsiv_fill_expansion(cv::Mat const& in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO:
    //Use of cv::copyMakeBorder is not allowed.
    //Hint you don't need use any for sentence.
    ret_v = cv::Mat::zeros(in.rows+2*r, in.cols+2*r, in.type());

    cv::Range rows(r, r+in.rows);
    cv::Range cols(r, r+in.cols);

    in.copyTo(ret_v(rows, cols));

    //
    CV_Assert(ret_v.type()==in.type());
    CV_Assert(ret_v.rows == in.rows+2*r);
    CV_Assert(ret_v.cols == in.cols+2*r);
    return ret_v;
}

cv::Mat
fsiv_circular_expansion(cv::Mat const& in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO
    // Use of cv::copyMakeBorder is not allowed.
    // Hint you don't need use any "for" sentence, only 9 copyTo from "in"
    // rois to "ret_v" rois.
    ret_v = cv::Mat::zeros(in.rows+2*r, in.cols+2*r, in.type());

    in(cv::Rect(0,in.rows-r,in.cols,r)).copyTo(ret_v(cv::Rect(r, 0, in.cols,r)));
    in(cv::Rect(0,0,in.cols,r)).copyTo(ret_v(cv::Rect(r,ret_v.rows-r, in.cols,r)));

    in(cv::Rect(in.cols-r,0,r,in.rows)).copyTo(ret_v(cv::Rect(0,r,r,in.rows)));
    in(cv::Rect(0,0,r,in.rows)).copyTo(ret_v(cv::Rect(ret_v.cols-r,r,r,in.rows)));

    in(cv::Rect(in.cols-r,in.rows-r,r,r)).copyTo(ret_v(cv::Rect(0,0,r,r)));
    in(cv::Rect(0,in.rows-r,r,r)).copyTo(ret_v(cv::Rect(ret_v.cols-r,0,r,r)));
    in(cv::Rect(in.cols-r,0,r,r)).copyTo(ret_v(cv::Rect(0,ret_v.rows-r,r,r)));
    in(cv::Rect(0,0,r,r)).copyTo(ret_v(cv::Rect(ret_v.cols-r,ret_v.rows-r,r,r)));

    in.copyTo(ret_v(cv::Rect(r,r,in.cols,in.rows)));

    //
    CV_Assert(ret_v.type()==in.type());
    CV_Assert(ret_v.rows == in.rows+2*r);
    CV_Assert(ret_v.cols == in.cols+2*r);
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,0)==in.at<uchar>(in.rows-r, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,ret_v.cols/2)==in.at<uchar>(in.rows-r, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,ret_v.cols-1)==in.at<uchar>(in.rows-r, r-1));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows/2,0)==in.at<uchar>(in.rows/2, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows/2,ret_v.cols/2)==in.at<uchar>(in.rows/2, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,0)==in.at<uchar>(r-1, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,ret_v.cols/2)==in.at<uchar>(r-1, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,ret_v.cols-1)==in.at<uchar>(r-1, r-1));
    return ret_v;
}

cv::Mat
fsiv_filter2D(cv::Mat const& in, cv::Mat const& filter)
{
    CV_Assert(!in.empty() && !filter.empty());
    CV_Assert(in.type()==CV_32FC1 && filter.type()==CV_32FC1);
    cv::Mat ret_v;

    // TODO
    // Remember: use of cv::filter2D is not allowed.

    //No se puede aplicar a los pixeles exteriores
    ret_v = cv::Mat::zeros(in.rows-filter.rows, in.cols-filter.rows, in.type());

    for(int i = 0; i < ret_v.rows; i++){
        for(int j = 0; j < ret_v.cols; j++){
            //La suma de cada uno de los pieles del filtro * los de in en esa ventana
            ret_v.at<float>(i,j) = sum(filter.mul(in(cv::Rect(i, j, filter.cols, filter.rows)))).val[0];
        }
    }

    

    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==in.rows-2*(filter.rows/2));
    CV_Assert(ret_v.cols==in.cols-2*(filter.cols/2));
    return ret_v;
}

cv::Mat
fsiv_combine_images(const cv::Mat src1, const cv::Mat src2,
                    double a, double b)
{
    CV_Assert(src1.type()==src2.type());
    CV_Assert(src1.rows==src2.rows);
    CV_Assert(src1.cols==src2.cols);
    cv::Mat ret_v;
    
    //TODO
    // Remember: use vectorized code.


    //
    CV_Assert(ret_v.type()==src2.type());
    CV_Assert(ret_v.rows==src2.rows);
    CV_Assert(ret_v.cols==src2.cols);
    return ret_v;
}

cv::Mat
fsiv_usm_enhance(cv::Mat  const& in, double g, int r,
                 int filter_type, bool circular, cv::Mat *unsharp_mask)
{
    CV_Assert(!in.empty());
    CV_Assert(in.type()==CV_32FC1);
    CV_Assert(r>0);
    CV_Assert(filter_type>=0 && filter_type<=1);
    CV_Assert(g>=0.0);
    cv::Mat ret_v;
    // TODO
    // Remember: use your own functions fsiv_xxxx
    // Remember: when unsharp_mask pointer is nullptr, means don't save the
    // unsharp mask on in.




    //
    CV_Assert(ret_v.rows==in.rows);
    CV_Assert(ret_v.cols==in.cols);
    CV_Assert(ret_v.type()==CV_32FC1);
    return ret_v;
}
