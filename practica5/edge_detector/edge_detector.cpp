#include <iostream>
#include <exception>

//Includes para OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const char * keys =
    "{help h usage ? |      | print this message   }"
    "{i              |      | Activate interactive mode.}"
    "{s_ap           | 1    | Sobel kernel aperture radio: 0, 1, 2, 3}"
    "{n_bins         | 100  | Gradient histogram size}"
    "{g_r            | 1    | radius of gaussian filter (2r+1). Value 0 means don't filter.}"
    "{th             | 0.8  | Gradient percentile used as threshold for the gradient percentile detector (th2 for canny).}"
    "{th1            | 0.2  | Gradient percentile used as th1 threshold for the Canny detector (th1 < th).}"
    "{m method       | 0    | Detector used: 0:percentile detector, 1:Otsu detector, 2:canny detector}"
    "{c consensus    | 0    | If a ground truth was given, use greater to c% consensus to generate ground truth.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    "{@ground_truth  |      | optional ground truth image to compute the detector metrics.}"
    ;


int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Edge detector v0.0");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      cv::String input_fname = parser.get<cv::String>("@input");
      cv::String output_fname = parser.get<cv::String>("@output");
      cv::String gt_fname = parser.get<cv::String>("@ground_truth");
      int n_bins = parser.get<int>("n_bins");
      int g_r = parser.get<int>("g_r");
      float th2 = parser.get<float>("th");
      float th1 = parser.get<float>("th1");
      int s_ap = parser.get<int>("s_ap");
      int method = parser.get<int>("method");
      float consensus = parser.get<float>("c");
      bool interactive = parser.has("i");

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

    cv::Mat img = cv::imread(input_fname, cv::IMREAD_GRAYSCALE);    
    cv::Mat gt_img;
    if (gt_fname != "")
      gt_img = cv::imread(gt_fname, cv::IMREAD_GRAYSCALE);

    // TODO
    // Remember: you must threshold gt image (>consensus) to get a 0/255 ground-truth.        

    //

  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "Capturada excepcion desconocida!" << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
