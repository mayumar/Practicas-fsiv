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
    "{v video        |      | the input is a video file.}"
    "{fourcc         |      | output video codec used, for example \"MJPG\". Default same as input.}"
    "{@intrinsics    |<none>| intrinsics parameters file.}"
    "{@input         |<none>| input image|video.}"
    "{@output        |<none>| output image|video.}"
    ;


int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Undistort an image or video file.");
    if (parser.has("help"))
    {
        parser.printMessage();
        return EXIT_SUCCESS;
    }
    auto is_video = parser.has("v");
    auto calib_fname = parser.get<std::string>("@intrinsics");
    auto input_fname = parser.get<std::string>("@input");
    auto output_fname = parser.get<std::string>("@output");
    if (!parser.check())
    {
        parser.printErrors();
        return EXIT_FAILURE;
    }

    try {

        float error;
        cv::Size camera_size;
        cv::Mat K, dist_coeffs, rvec, tvec;

        //TODO: First load the calibration parameters.
        cv::FileStorage fs(calib_fname, cv::FileStorage::Mode::READ);
        fsiv_load_calibration_parameters(fs, camera_size, error, K, dist_coeffs, rvec, tvec);
        //

        cv::namedWindow("INPUT", cv::WINDOW_GUI_EXPANDED+cv::WINDOW_AUTOSIZE);
        cv::namedWindow("OUTPUT", cv::WINDOW_GUI_EXPANDED+cv::WINDOW_AUTOSIZE);
        //TODO

        //
        if (is_video)
        {
            //TODO
            cv::VideoCapture input(input_fname);
            int fourcc = input.get(cv::CAP_PROP_FOURCC);
            if(parser.has("fourcc")){
                fourcc = parser.get<int>("fourcc");
            }
            double fps = input.get(cv::CAP_PROP_FPS);
            cv::VideoWriter output(output_fname, fourcc, fps, camera_size);
            fsiv_undistort_video_stream(input, output, K, dist_coeffs, 1, "INPUT", "OUTPUT", fps);
            //
        }
        else
        {
            //TODO
            cv::Mat input = cv::imread(input_fname);
            cv::Mat output = input.clone();
            fsiv_undistort_image(input, output, K, dist_coeffs);

            cv::imshow("INPUT", input);
            cv::imshow("OUTPUT", output);

            int key = 0;
            while(key!=27){
                key = cv::waitKey(0) & 0xff;
            }
            cv::destroyAllWindows();
            
            cv::imwrite(output_fname, output);
            //
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}
