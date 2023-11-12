/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const char* keys =
    "{help h usage ? |      | print this message.}"
    "{i interactive  |      | Activate interactive mode.}"
    "{l luma         |      | process only \"luma\" if color image.}"
    "{f filter       |0     | filter to use.}"
    "{r1             |1     | r1 for DoG filter.}"
    "{r2             |2     | r2 for DoG filter. (0<r1<r2)}"
    "{c circular     |      | use circular convolution.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;

struct UserData
{
    cv::Mat input;
    cv::Mat output;
    int luma;
    int filter;
    int r1;
    int r2;
    int circular;
};

void on_change_l(int luma, void * user_data_){
  UserData * user_data = static_cast<UserData*>(user_data_);
  user_data->luma = luma;
  
  user_data->output = fsiv_image_sharpening(user_data->input, user_data->filter, luma, user_data->r1, user_data->r2, user_data->circular);

  cv::imshow("OUTPUT", user_data->output);
}

void on_change_f(int filter, void * user_data_){
  UserData * user_data = static_cast<UserData*>(user_data_);
  user_data->filter = filter;
  
  user_data->output = fsiv_image_sharpening(user_data->input, filter, user_data->luma, user_data->r1, user_data->r2, user_data->circular);

  cv::imshow("OUTPUT", user_data->output);
}

void on_change_r1(int r1, void * user_data_){
  UserData * user_data = static_cast<UserData*>(user_data_);
  user_data->r1 = r1;
  
  user_data->output = fsiv_image_sharpening(user_data->input, user_data->filter, user_data->luma, r1, user_data->r2, user_data->circular);

  cv::imshow("OUTPUT", user_data->output);
}

void on_change_r2(int r2, void * user_data_){
  UserData * user_data = static_cast<UserData*>(user_data_);
  user_data->r2 = r2;
  
  user_data->output = fsiv_image_sharpening(user_data->input, user_data->filter, user_data->luma, user_data->r1, r2, user_data->circular);

  cv::imshow("OUTPUT", user_data->output);
}

void on_change_c(int circular, void * user_data_){
  UserData * user_data = static_cast<UserData*>(user_data_);
  user_data->circular = circular;
  
  user_data->output = fsiv_image_sharpening(user_data->input, user_data->filter, user_data->luma, user_data->r1, user_data->r2, circular);

  cv::imshow("OUTPUT", user_data->output);
}

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Enhance an image using a sharpening filter. (ver 0.0.0)");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }

      cv::String input_name = parser.get<cv::String>(0);
      cv::String output_name = parser.get<cv::String>(1);

      //TODO
      //CLI parameters.
      bool i = parser.get<bool>("interactive");
      bool luma = parser.get<bool>("luma");
      int filter = parser.get<int>("filter");
      int r1 = parser.get<int>("r1");
      int r2 = parser.get<int>("r2");
      bool circular = parser.get<bool>("circular");
      //

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

      cv::Mat input = cv::imread(input_name);

      if (input.empty())
	  {
		  std::cerr << "Error: could not open the input image '" << input_name << "'." << std::endl;
		  return EXIT_FAILURE;
	  }
      cv::Mat output = input.clone();

      //TODO

      cv::namedWindow("INPUT");
      cv::namedWindow("OUTPUT");

      UserData user_data;

      if(!i){
        output = fsiv_image_sharpening(input, filter, luma, r1, r2, circular);
      }else{
        user_data.input = input;
        user_data.output = output;
        int l_int = (int)luma;
        user_data.luma = l_int;
        user_data.filter = filter;
        user_data.r1 = r1;
        user_data.r2 = r2;
        int c_int = (int)circular;
        user_data.circular = c_int;
        cv::createTrackbar("L", "OUTPUT", &l_int, 1, on_change_l, &user_data);
        cv::createTrackbar("F", "OUTPUT", &filter, 2, on_change_f, &user_data);
        cv::createTrackbar("R1", "OUTPUT", &r1, 100, on_change_r1, &user_data);
        cv::createTrackbar("R2", "OUTPUT", &r2, 100, on_change_r2, &user_data);
        cv::createTrackbar("C", "OUTPUT", &c_int, 1, on_change_c, &user_data);
      }

      //

      // cv::namedWindow("INPUT");
      // cv::namedWindow("OUTPUT");


      cv::imshow("INPUT", input);
      cv::imshow("OUTPUT", output);


      int key = cv::waitKey(0) & 0xff;

      //TODO
      //Write the result if it's asked for.
      if (key!=27)
        {
            if(!cv::imwrite(output_name, output)){
                std::cout << "Error: could not save the result in file '" << output_name << "'" << std::endl;
            }
        }
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
