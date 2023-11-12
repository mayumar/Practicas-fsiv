/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const cv::String keys =
    "{help h usage ? |      | print this message.}"
    "{i interactive  |      | Interactive mode.}"
    "{r radius       |1     | Window's radius. Default 1.}"
    "{g gain         |1.0   | Enhance's gain. Default 1.0}"
    "{c circular     |      | Use circular convolution.}"
    "{f filter       |0     | Filter type: 0->Box, 1->Gaussian. Default 0.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;

struct UserData
{
    cv::Mat input;
    cv::Mat output;
    cv::Mat mask;
    int r;
    int g;
    int circular;
    int filter_type;
};

void on_change_r(int r, void * user_data_){
    UserData * user_data = static_cast<UserData*>(user_data_);
    user_data->r = r;

    cv::Mat in_convert;
    (user_data->input).convertTo(in_convert, CV_32FC1, 1/255.0);
    if((user_data->input).channels() == 3){
        cv::Mat in_hsv;
        cv::cvtColor(in_convert, in_hsv, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> channels;
        cv::split(in_hsv, channels);
        channels[2] = fsiv_usm_enhance(channels[2], (user_data->g)/100.0, r, user_data->filter_type, user_data->circular, &(user_data->mask));
        cv::merge(channels, in_hsv);

        cv::cvtColor(in_hsv, user_data->output, cv::COLOR_HSV2BGR);

    }else{
        user_data->output = fsiv_usm_enhance(in_convert, (user_data->g)/100.0, r, user_data->filter_type, user_data->circular, &(user_data->mask));
    }

    cv::imshow("OUTPUT", user_data->output);
    cv::imshow("UNSHARP MASK", user_data->mask);
}

void on_change_g(int g, void * user_data_){
    UserData * user_data = static_cast<UserData*>(user_data_);
    user_data->g = g;

    cv::Mat in_convert;
    (user_data->input).convertTo(in_convert, CV_32FC1, 1/255.0);
    if((user_data->input).channels() == 3){
        cv::Mat in_hsv;
        cv::cvtColor(in_convert, in_hsv, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> channels;
        cv::split(in_hsv, channels);
        channels[2] = fsiv_usm_enhance(channels[2], g/100, user_data->r, user_data->filter_type, user_data->circular, &(user_data->mask));
        cv::merge(channels, in_hsv);

        cv::cvtColor(in_hsv, user_data->output, cv::COLOR_HSV2BGR);

    }else{
        user_data->output = fsiv_usm_enhance(in_convert, g/100, user_data->r, user_data->filter_type, user_data->circular, &(user_data->mask));
    }

    cv::imshow("OUTPUT", user_data->output);
    cv::imshow("UNSHARP MASK", user_data->mask);
}

void on_change_c(int circular, void * user_data_){
    UserData * user_data = static_cast<UserData*>(user_data_);
    user_data->circular = circular;

    cv::Mat in_convert;
    (user_data->input).convertTo(in_convert, CV_32FC1, 1/255.0);
    if((user_data->input).channels() == 3){
        cv::Mat in_hsv;
        cv::cvtColor(in_convert, in_hsv, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> channels;
        cv::split(in_hsv, channels);
        channels[2] = fsiv_usm_enhance(channels[2], (user_data->g)/100.0, user_data->r, user_data->filter_type, circular, &(user_data->mask));
        cv::merge(channels, in_hsv);

        cv::cvtColor(in_hsv, user_data->output, cv::COLOR_HSV2BGR);

    }else{
        user_data->output = fsiv_usm_enhance(in_convert, (user_data->g)/100.0, user_data->r, user_data->filter_type, circular, &(user_data->mask));
    }

    cv::imshow("OUTPUT", user_data->output);
    cv::imshow("UNSHARP MASK", user_data->mask);
}

void on_change_f(int filter_type, void * user_data_){
    UserData * user_data = static_cast<UserData*>(user_data_);
    user_data->filter_type = filter_type;

    cv::Mat in_convert;
    (user_data->input).convertTo(in_convert, CV_32FC1, 1/255.0);
    if((user_data->input).channels() == 3){
        cv::Mat in_hsv;
        cv::cvtColor(in_convert, in_hsv, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> channels;
        cv::split(in_hsv, channels);
        channels[2] = fsiv_usm_enhance(channels[2], (user_data->g)/100.0, user_data->r, filter_type, user_data->circular, &(user_data->mask));
        cv::merge(channels, in_hsv);

        cv::cvtColor(in_hsv, user_data->output, cv::COLOR_HSV2BGR);

    }else{
        user_data->output = fsiv_usm_enhance(in_convert, (user_data->g)/100.0, user_data->r, filter_type, user_data->circular, &(user_data->mask));
    }

    cv::imshow("OUTPUT", user_data->output);
    cv::imshow("UNSHARP MASK", user_data->mask);
}

int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try {
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Apply an unsharp mask enhance to an image.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }

        cv::String input_n = parser.get<cv::String>("@input");
        cv::String output_n = parser.get<cv::String>("@output");
        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }

        //TODO
        double g = 1.0;
        int r = 1;
        int filter_type=1;
        bool circular =false;

        bool i = parser.get<bool>("interactive");
        g = parser.get<double>("gain");
        r = parser.get<int>("radius");
        filter_type = parser.get<int>("filter");
        circular = parser.get<bool>("circular");

        //

        cv::Mat in = cv::imread(input_n, cv::IMREAD_UNCHANGED);
        cv::Mat out = in.clone();
        cv::Mat mask = in.clone();

        if (in.empty())
        {
            std::cerr << "Error: could not open input image '" << input_n
                      << "'." << std::endl;
            return EXIT_FAILURE;
        }

        cv::namedWindow("INPUT");
        cv::namedWindow("OUTPUT");
        cv::namedWindow("UNSHARP MASK");

        //
        UserData user_data;

        if(!i){
            cv::Mat in_convert;
            in.convertTo(in_convert, CV_32FC1, 1/255.0);

            if(in.channels() == 3){
                cv::Mat in_hsv;
                cv::cvtColor(in_convert, in_hsv, cv::COLOR_BGR2HSV);
        
                std::vector<cv::Mat> channels;
                cv::split(in_hsv, channels);
                channels[2] = fsiv_usm_enhance(channels[2], g, r, filter_type, circular, &mask);
                cv::merge(channels, in_hsv);

                cv::cvtColor(in_hsv, out, cv::COLOR_HSV2BGR);

            }else{
                out = fsiv_usm_enhance(in_convert, g, r, filter_type, circular, &mask);
            }
        }else{
            user_data.input = in;
            user_data.output = out;
            user_data.r = r;
            int g_int = (int)g*100;
            user_data.g = g_int;
            int circular_int = (int)circular;
            user_data.circular = circular_int;
            user_data.filter_type = filter_type;
            user_data.mask = mask;
            cv::createTrackbar("R", "INPUT", &r, 100, on_change_r, &user_data);
            cv::createTrackbar("G", "INPUT", &g_int, 500, on_change_g, &user_data);
            cv::createTrackbar("C", "INPUT", &circular_int, 1, on_change_c, &user_data);
            cv::createTrackbar("F", "INPUT", &filter_type, 1, on_change_f, &user_data);
        }
        //
        //out = fsiv_usm_enhance(in, g, r, filter_type, circular, &mask);


        cv::imshow ("INPUT", in);
        cv::imshow ("OUTPUT", out);
        cv::imshow ("UNSHARP MASK", mask);

        int k = cv::waitKey(0)&0xff;
        if (k!=27)
            cv::imwrite(output_n, out);
    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}
