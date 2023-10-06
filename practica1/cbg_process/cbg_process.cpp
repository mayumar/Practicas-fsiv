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
    "{i interactive  |      | Activate interactive mode.}"
    "{l luma         |      | process only \"luma\" if color image.}"
    "{c contrast     |1.0   | contrast parameter.}"
    "{b bright       |0.0   | bright parameter.}"
    "{g gamma        |1.0   | gamma parameter.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;

int c, c_max = 200;
int b, b_max = 200;
int g, g_max = 200;
int l, l_max = 1;
cv::Mat input;
cv::Mat output;
float contrast;
float bright;
float gam;

void on_trackbar(int, void*){
  contrast = c/100.0;
  bright = (b/100.0)-1;
  gam = g/100.0;
  fsiv_cbg_process(input, output, contrast, bright, gam, l);
  cv::imshow("PROCESADA", output);
  
}

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Ajust the contrast/bright/gamma parameters of an image. (ver 0.0.0)");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }

      cv::String input_name = parser.get<cv::String>(0);
      cv::String output_name = parser.get<cv::String>(1);

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

      cv::namedWindow("ORIGINAL");
      cv::namedWindow("PROCESADA");


      //TODO
      bool interactive = parser.get<bool>("i");
      bool luma = parser.get<bool>("l");
      contrast = parser.get<float>("c");
      bright = parser.get<float>("b");
      float gamma = parser.get<float>("g");

      input = cv::imread(input_name, cv::IMREAD_ANYCOLOR);

      if(input.empty()){
        std::cerr << "Error: no se ha podido abrir el fichero " << input_name << std::endl;
        return EXIT_FAILURE;
      }

      if(!interactive){
        fsiv_cbg_process(input, output, contrast, bright, gamma, luma);
      }else{
        c = (int)(contrast*100);
        g = (int)((bright+1)*100);
        g = (int)(gamma*100);
        l = (int)luma;
        cv::createTrackbar("Contrast", "PROCESADA", &c, c_max, on_trackbar);
        cv::createTrackbar("Bright", "PROCESADA", &b, b_max, on_trackbar);
        cv::createTrackbar("Gamma", "PROCESADA", &g, g_max, on_trackbar);
        cv::createTrackbar("Luma", "PROCESADA", &l, l_max, on_trackbar);
      }

      cv::imshow("ORIGINAL", input);
      cv::imshow("PROCESADA", output);

      int key = cv::waitKey(0) & 0xff;

      if (key != 27)
      {
          if (!cv::imwrite(output_name, output))
          {
              std::cerr << "Error: could not save the result in file '"<<output_name<<"'."<< std::endl;
                return EXIT_FAILURE;
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


