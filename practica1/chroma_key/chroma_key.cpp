//! \file chroma_key.cpp
//! Replace background based on color
//! University of Cordoba
//! (c) MJMJ/2020 FJMC/2022-

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "common_code.hpp"

const char * keys =
    "{help usage ? |      | print this message   }"
    "{h hue          |  60  | Chroma key (hue). Def. 60}"
    "{s sensitivity  |  20 | sensitivity. Def. 20}"
    "{v video        |     | the input is a videofile.}"
    "{c camera       |     | the input is a capture device index.}"
    "{@input         |<none>| input source (pathname or camera idx).}"
    "{@background    |<none>| pathname of background image.}"
    ;






int
main(int argc, char * argv[])
{
    int retCode = EXIT_SUCCESS;
    try {
        /// Init the parser
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Replace background based on color.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return 0;
        }

        // Get the values
        int hue = parser.get<int>("h");
        int sens = parser.get<int>("s");
        bool video = parser.get<bool>("v");
        bool camera = parser.get<bool>("c");
        cv::String input = parser.get<cv::String>("@input"); 
        cv::String background = parser.get<cv::String>("@background");
        //

        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }



        //DO the work!!.

        cv::Mat backg = cv::imread(background, cv::IMREAD_ANYCOLOR);

        if(backg.empty()){
            std::cerr << "Error: no se ha podido abrir el fichero " << background << std::endl;
            return EXIT_FAILURE;
        }

        if(!video && !camera){

            cv::Mat foreg = cv::imread(input, cv::IMREAD_ANYCOLOR);

            if(foreg.empty()){
                std::cerr << "Error: no se ha podido abrir el fichero " << input << std::endl;
                return EXIT_FAILURE;
            }

            cv::Mat res = fsiv_apply_chroma_key(foreg, backg, hue, sens);

            cv::namedWindow("IMG", cv::WINDOW_GUI_EXPANDED);

            cv::imshow("IMG", res);

            std::cout << "Pulsa ESC para salir" << std::endl;
            while((cv::waitKey(0) & 0xff) != 27);

            cv::destroyWindow("IMG");

        }else if(!camera){

            cv::VideoCapture vidforg;
            vidforg.open(input);

            if(!vidforg.isOpened()){
                std::cerr << "Error: no he podido abrir el video " << input << std::endl;
                return EXIT_FAILURE;
            }

            cv::namedWindow("VIDEO");

            int key = cv::waitKey(0) & 0xff;
            cv::Mat frame;
            vidforg >> frame;

            while(!frame.empty() && key!=27){
                cv::Mat output = fsiv_apply_chroma_key(frame, backg, hue, sens);
                cv::imshow("VIDEO", output);

                key = cv::waitKey(25) & 0xff;
                vidforg >> frame;
            }

            cv::destroyWindow("VIDEO");

        }else{

            cv::VideoCapture camforg;
            camforg.open(std::stoi(input));

            if(!camforg.isOpened()){
                std::cerr << "Error: no se ha podido abrir la fuente de video" << std::endl;
                return EXIT_FAILURE;
            }

            cv::namedWindow("VIDEO");

            int key = cv::waitKey(0) & 0xff;
            cv::Mat frame;
            camforg >> frame;

            if(frame.empty()){
                std::cerr << "Error: no se ha podido abrir la fuente de video" << std::endl;
                return EXIT_FAILURE;
            }

            while(!frame.empty() && key!=27){
                cv::Mat output = fsiv_apply_chroma_key(frame, backg, hue, sens);
                cv::imshow("VIDEO", output);

                key = cv::waitKey(25) & 0xff;
                camforg >> frame;
            }

            cv::destroyWindow("VIDEO");

        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown excepcion." << std::endl;
        retCode = EXIT_FAILURE;
    }

    return retCode;
}
