#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>

#include "common_code.hpp"

const cv::String keys =
    "{help h usage ? |      | print this message   }"
    "{c              |      | The input video stream is a camera.}"
    "{ste_r          |3     | Structure element radius for morph. Default 0 means not use.}"
    "{ste_type       |2     | Type of structure element: 0:rect, 1:cross, 2:ellipse.}"
    "{gaussian       |      | Use gaussian blur, else use uniform kernel.}"
    "{th             |2.0   | Threshold the optical flow magnitude to get the mask.}"
    "{alpha          |0.2   | The mask has an alpha memory factor. Default 0 means don't have memory.}"
    "{blur_r         |5     | Blur radius. Default 11.}"
    "{@input         |      | input stream (filename or camera idx)}"
    ;



int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try
    {

        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Blur the background of the input video stream using optical flow magnitude as clue.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }
        bool is_camera = parser.has("c");
        int blur_r = parser.get<int>("blur_r");

        cv::Mat ste;
        int ste_r = parser.get<int>("ste_r");
        int ste_type = parser.get<int>("ste_type");
        

        bool use_gaussian_blur = parser.has("gaussian");
        double th = parser.get<double>("th");
        double alpha = parser.get<double>("alpha");

        int camera_idx = -1;
        std::string input_file;
        if (is_camera)
            camera_idx = std::stoi(parser.get<std::string>("@input"));
        else
            input_file = parser.get<std::string>("@input");
        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }
        
        cv::VideoCapture cap;

        if (is_camera)
            ;
        else
            cap.open(input_file);

        if (!cap.isOpened())
        {
            std::cerr << "Error: could not open the input stream!" << std::endl;
            return EXIT_FAILURE;
        }

        // TODO


    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}
