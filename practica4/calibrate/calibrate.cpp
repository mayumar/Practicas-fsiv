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
    "{verbose        |      | activate verbose mode.}"
    "{i intrinsics   |      | Calibrate only extrinsics parameters. Using intrinsics from given file (-i=intr-file).}"
    "{s size         |<none>| square size.}"
    "{r rows         |<none>| number of board's rows.}"
    "{c cols         |<none>| number of board's cols.}"
    "{@output        |<none>| filename for output intrinsics file.}"
    "{@input1        |<none>| first board's view.}"
    "{@input2        |      | second board's view.}"
    "{@inputn        |      | ... n-idx board's view.}"
    ;

int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try {        
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Calibrate the intrinsics parameters of a camera.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }
        float square_size = parser.get<float>("s");
        int rows = parser.get<int>("r");
        int cols = parser.get<int>("c");
        bool verbose = parser.has("verbose");
        std::string output_fname = parser.get<cv::String>("@output");
        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }

        //Get the inputs.
        //find the second argument without '-' at begin.
        int input = 1;
        bool found = false;
        while (input<argc && !found)
            found = argv[input++][0] != '-';
        CV_Assert(input<argc);
        std::vector<std::string> input_fnames;        
        for (; input<argc; ++input)
            input_fnames.push_back(std::string(argv[input]));

        //TODO




        if (parser.has("i"))
        {
            //TODO
            //Make extrinsic calibration.
            //Remenber: only one view is needed.
            std::string intrins_name = parser.get<std::string>("i");
            cv::FileStorage fs(intrins_name, cv::FileStorage::Mode::READ);
            cv::Size camera_size;
            float error;
            cv::Mat camera_matrix, dist_coeffs, rvec, tvec;
            fsiv_load_calibration_parameters(fs, camera_size, error, camera_matrix, dist_coeffs, rvec, tvec);
            cv::Mat input1 = cv::imread(input_fnames[0]);
            std::vector<cv::Point2f> corner_points;
            cv::namedWindow("POINTS");
            bool wasfound = fsiv_find_chessboard_corners(input1, cv::Size(cols-1,rows-1), corner_points, "POINTS");
            std::vector<cv::Point3f> _3dpoints;
            if(wasfound){
                _3dpoints = fsiv_generate_3d_calibration_points(cv::Size(cols-1,rows-1), square_size);
                fsiv_compute_camera_pose(_3dpoints, corner_points, camera_matrix, dist_coeffs, rvec, tvec);
            }


            //
            if (verbose)
            {
                //TODO
                //Show WCS axis.
                cv::namedWindow("VIEW");    
                fsiv_draw_axes(input1, camera_matrix, dist_coeffs, rvec, tvec, square_size, 3);
                cv::imshow("VIEW", input1);
                int key = 0;
                while(key!=27){
                    key = cv::waitKey(0) & 0xff;
                }
                cv::destroyAllWindows();
                //
            }
        }
        else
        {
            //TODO
            //Make an intrisic calibration.
            //Remember: For each view (at least two) you must find the
            //chessboard to get the 3D -> 2D matches.
            std::vector<std::vector<cv::Point3f>> _3dpoints;
            std::vector<std::vector<cv::Point2f>> corner_points(input_fnames.size());
            std::vector<cv::Mat> input_imgs;
            
            cv::namedWindow("POINTS");
            for(size_t i = 0; i < input_fnames.size(); i++){
                input_imgs.push_back(cv::imread(input_fnames[i]));
                fsiv_find_chessboard_corners(input_imgs[i], cv::Size(cols-1,rows-1), corner_points[i], "POINTS");
                _3dpoints.push_back(fsiv_generate_3d_calibration_points(cv::Size(cols-1,rows-1), square_size));
            }
            
            cv::Mat camera_matrix, dist_coeffs;
            std::vector<cv::Mat> rvecs, tvecs;
            fsiv_calibrate_camera(corner_points, _3dpoints, input_imgs[0].size(), camera_matrix, dist_coeffs, &rvecs, &tvecs);

            for(size_t i = 0; i < input_imgs.size(); i++)
                fsiv_compute_camera_pose(_3dpoints[i], corner_points[i], camera_matrix, dist_coeffs, rvecs[i], tvecs[i]);
            //

            if (verbose)
            {
                //TODO
                //Show WCS axis on each pattern view.
                cv::namedWindow("VIEW");
                for(size_t i = 0; i < input_imgs.size(); i++){
                    fsiv_draw_axes(input_imgs[i], camera_matrix, dist_coeffs, rvecs[i], tvecs[i], square_size, 3);
                    cv::imshow("VIEW", input_imgs[i]);
                    int key = 0;
                    while(key!=27){
                        key = cv::waitKey(0) & 0xff;
                    }
                }
                cv::destroyAllWindows();
                //
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
