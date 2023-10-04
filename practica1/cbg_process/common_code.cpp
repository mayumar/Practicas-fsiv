#include "common_code.hpp"

cv::Mat
fsiv_convert_image_byte_to_float(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.depth()==CV_8U);
    //TODO
    img.convertTo(out, CV_32F, 1/255.0, 0.0);
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
    img.convertTo(out, CV_8U, 255, 0);
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
    cv::cvtColor(img, out, cv::COLOR_BGR2HSV);
    //
    CV_Assert(out.channels()==3);
    return out;
}

cv::Mat
fsiv_convert_hsv_to_bgr(const cv::Mat& img, cv::Mat& out)
{
    CV_Assert(img.channels()==3);
    //TODO
    cv::cvtColor(img, out, cv::COLOR_HSV2BGR);
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
    
    cv::Mat inF;
    fsiv_convert_image_byte_to_float(in, inF);

    if(!only_luma){
        
        std::vector<cv::Mat> canales;
        std::vector<cv::Mat> canalesout;
        cv::split(inF, canales);

        for(size_t i = 0; i < canales.size(); i++){
            cv::Mat auxpow;
            cv::Mat auxRGB;
            cv::pow(canales[i], gamma, auxpow);
            auxRGB = contrast * auxpow + brightness;
            canalesout.push_back(auxRGB);
        }

        cv::merge(canalesout, inF);
        
        
    }else{
        
        cv::Mat inHSV;
        fsiv_convert_bgr_to_hsv(inF, inHSV);

        std::vector<cv::Mat> HSV;
        cv::split(inHSV, HSV);

        cv::Mat auxpow;
        cv::pow(HSV[2], gamma, auxpow);
        HSV[2] = contrast * auxpow + brightness;

        cv::merge(HSV, inHSV);

        fsiv_convert_hsv_to_bgr(inHSV, inF);
    }

    fsiv_convert_image_float_to_byte(inF, out);


    //
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    CV_Assert(out.depth()==CV_8U);
    CV_Assert(out.channels()==in.channels());
    return out;
}
