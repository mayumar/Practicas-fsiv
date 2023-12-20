/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <string>
#include <exception>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "dataset.hpp"

const char * keys =
    "{help h usage ? |      | print this message   }"
    "{@data          |./mnist/| folder with the dataset.}"
    ;

static const int LEFT_ARROW = 81;
static const int RIGHT_ARROW = 83;


int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;

  try {
      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Load the fashion mnist dataset (binary format). "
                   "You can download it from: https://www.kaggle.com/datasets/zalando-research/fashionmnist");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }
      std::string folder = parser.get<std::string>("@data");
      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }
      
      std::cout << "Loading data from folder: " << folder << std::endl;
      
      cv::Mat X, y;
      fsiv_load_dataset(folder, 0, X, y);
      const int dataset_size = X.rows;
      std::cout << "Loaded " << dataset_size << " images." << std::endl;
      
      int key = 0;
      int idx = 0;
      std::string wname = "IMAGE";
      cv::namedWindow(wname, cv::WINDOW_GUI_EXPANDED);
      cv::resizeWindow(wname, cv::Size(256,256));
      do
      {          
          cv::Mat img = X.row(idx).reshape(1, 28);
          cv::imshow(wname, img);
          std::cout << "Idx " << idx << ": "                    
                    << fsiv_get_dataset_label_name(y.at<std::uint32_t>(idx))
                    << std::endl;
          key = cv::waitKey(0) & 0xff;
          if (key == LEFT_ARROW)
              idx = (idx-1+dataset_size) % dataset_size;
          else if  (key == RIGHT_ARROW)
              idx = (idx+1) % dataset_size;
          else if (key != 27)
              std::cout << "Unknown keypress code '" << key
                        << "' [Press <-, ->, or ESC]." << std::endl;
      }
      while (key != 27);

      cv::destroyWindow(wname);
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
