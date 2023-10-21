#include <iostream>
#include "common_code.hpp"

cv::Mat
fsiv_compute_histogram(const cv::Mat& in, cv::Mat& hist)
{
    CV_Assert(in.type()==CV_8UC1);
    CV_Assert(hist.empty() ||(hist.type()==CV_32FC1 &&
                              hist.rows==256 && hist.cols==1));
    //TODO
    //Tienes dos alternativas:
    //1- Implementar un recorrido por la imagen y calcular el histograma.
    //2- Usar la función cv::calcHist.
    //Sugerencia: implementa las dos para comparar.
    
    int hist_size = 256;
    float channel[] = {0, 256};
    const float * ranges[] = {channel};
    cv::calcHist(&in, 1, 0, cv::Mat(), hist, 1, &hist_size, ranges);

    //

    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);
    return hist;
}

void
fsiv_normalize_histogram(cv::Mat& hist)
{
    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);

    //TODO

    cv::normalize(hist, hist, 1.0, 0.0, cv::NORM_L1);

    //
    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);
    CV_Assert(cv::sum(hist)[0]==0.0 || cv::abs(cv::sum(hist)[0]-1.0)<=1.0e-6);
}

void
fsiv_accumulate_histogram(cv::Mat& hist)
{
    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);
#ifndef NDEBUG
    double old_sum = cv::sum(hist)[0];
#endif


    //TODO

    for(int i = 0; i < 256; i++){
        hist.at<float>(i) += hist.at<float>(i - 1); //Sumas el anterior
    }

    //

    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);
    CV_Assert((old_sum<=0.0) ||
              ((std::abs(old_sum-hist.at<float>(255))/old_sum)<=1.0e-5));
}

cv::Mat
fsiv_create_equalization_lookup_table(const cv::Mat& hist,
                                      bool hold_median)
{
    CV_Assert(hist.type()==CV_32FC1);
    CV_Assert(hist.rows==256 && hist.cols==1);
    cv::Mat lkt;
    //TODO
    //Usa las funciones fsiv_normalize_histogram y fsiv_accumulate_histogram
    //para construir la tabla.

    cv::Mat naHist;
    hist.copyTo(naHist);

    fsiv_normalize_histogram(naHist);
    fsiv_accumulate_histogram(naHist);
    
    if(!hold_median){
        naHist.convertTo(lkt, CV_8UC1, 255, 0);
    }else{
        int median = 0;
        float val = 0;

        while(val < 0.5 && median < 256){
            median++;
            val = naHist.at<float>(median);
        }

        cv::Range first_half(0, median), second_half(median, 256);

        naHist(first_half, cv::Range::all()) /= val;
        naHist(first_half, cv::Range::all()) *= median;

        naHist(second_half, cv::Range::all()) -= val;
        naHist(second_half, cv::Range::all()) /= 1-val;
        naHist(second_half, cv::Range::all()) *= 255-median;
        naHist(second_half, cv::Range::all()) += median;

        naHist.convertTo(lkt, CV_8UC1);
    }

    //

    CV_Assert(lkt.type()==CV_8UC1);
    CV_Assert(lkt.rows==256 && lkt.cols==1);
    return lkt;
}

cv::Mat
fsiv_apply_lookup_table(const cv::Mat&in, const cv::Mat& lkt,
                        cv::Mat& out)
{
    CV_Assert(in.type()==CV_8UC1);
    CV_Assert(lkt.type()==CV_8UC1);
    CV_Assert(lkt.rows==256 && lkt.cols==1);
    CV_Assert(out.empty() || (out.type()==CV_8UC1 &&
                              out.rows==in.rows && out.cols==in.cols));

    //TODO
    //Hint: you can use the cv::LUT function.
    cv::LUT(in, lkt, out);
    //
    CV_Assert(out.rows ==in.rows && out.cols==in.cols && out.type()==in.type());
    return out;
}

cv::Mat
fsiv_image_equalization(const cv::Mat& in, cv::Mat& out,
                    bool hold_median, int radius)
{
    CV_Assert(in.type()==CV_8UC1);
    //TODO
    //Utiliza las funciones fsiv_compute_histogram,
    //fsiv_create_equalization_lookup_table y fsiv_apply_lookup_table
    //Recuerda: cuando radius>0, la zona no procesada de la entrada se deja
    //   con los mismos valores en la salida.
    cv::Mat hist;
    
    if(radius == 0){
        fsiv_compute_histogram(in, hist);
        cv::Mat lut = fsiv_create_equalization_lookup_table(hist, hold_median);
        fsiv_apply_lookup_table(in, lut, out);
    }else{
        in.copyTo(out);

        for(int x = 0; x <= (in.rows-(2*radius+1)); x++){
            for(int y = 0; y <= (in.cols-(2*radius+1)); y++){
                cv::Range rangex(x, x+(2*radius+1));
                cv::Range rangey(y, y+(2*radius+1));
                cv::Mat ventana = in(rangex, rangey);
            
                fsiv_compute_histogram(ventana, hist);

                cv::Mat lut = fsiv_create_equalization_lookup_table(hist, hold_median);

                float res = lut.at<uchar>(in.at<uchar>(x+radius, y+radius));

                out.at<uchar>(x+radius, y+radius) = res;
                
            }
        }
    }
    //
    CV_Assert(out.rows==in.rows && out.cols==in.cols && out.type()==in.type());
    return out;
}
