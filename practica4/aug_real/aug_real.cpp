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
#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const cv::String keys =
    "{help h usage ? |      | print this message   }"
    "{m              |      | draw a 3d model else draw XYZ axis.}"
    "{c              |      | The input is a camera idx.}"
    "{i              |      | Render an image on the board. -i=img.png}"
    "{v              |      | Render a video on the board. -v=video.avi}"
    "{@rows          |<none>| board rows.}"
    "{@cols          |<none>| board cols.}"
    "{@size          |<none>| board squared side size.}"
    "{@intrinsics    |<none>| intrinsics filename.}"
    "{@input         |<none>| input stream (filename or camera idx)}"
    ;

int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try
    {

        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Do augmented reality on a input stream.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }
        int rows = parser.get<int>("@rows");
        int cols = parser.get<int>("@cols");
        cv::Size board_size(cols-1, rows-1);
        float size = parser.get<float>("@size");
        std::string intrinsics_file = parser.get<std::string>("@intrinsics");        
        bool is_camera = parser.has("c");
        bool draw_axis = !parser.has("m");
        int camera_idx = -1;
        std::string input_file;
        if (is_camera)
            camera_idx = parser.get<int>("@input");
        else
            input_file = parser.get<std::string>("@input");

        cv::Mat projected_image;
        if (parser.has("i"))        
        {
            projected_image=cv::imread(parser.get<std::string>("i"));
            if (projected_image.empty())
            {
                std::cerr << "Error: could not open image ["
                          << parser.get<std::string>("i") << "]." << std::endl;
                return EXIT_FAILURE;
            }
        }

        cv::VideoCapture projected_video;
        if (parser.has("v"))
        {
            if (!projected_video.open(parser.get<std::string>("v")))
            {
                std::cerr << "Error: could not open video ["
                          << parser.get<std::string>("v") << "]." << std::endl;
                return EXIT_FAILURE;
            }
        }

        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }
        
        cv::VideoCapture cap;
        if (is_camera)
            cap.open(camera_idx);
        else
            cap.open(input_file);

        if (!cap.isOpened())
        {
            std::cerr << "Error: could not open the input stream!" << std::endl;
            return EXIT_FAILURE;
        }
        

        //TODO
        //Load camera calibration parameters.
        cv::FileStorage fs(intrinsics_file, cv::FileStorage::Mode::READ);
        cv::Size camera_size;
        float error;
        cv::Mat camera_matrix, dist_coeffs, rvec, tvec;
        fsiv_load_calibration_parameters(fs, camera_size, error, camera_matrix, dist_coeffs, rvec, tvec);
        //


        //TODO
        //Compute the 3d coordinates of the board corners.
        cv::Mat input_frame, new_input_frame;
        cap >> input_frame;
        std::vector<cv::Point2f> corner_points;
        bool wasfound = fsiv_fast_find_chessboard_corners(input_frame, board_size, corner_points);
        if(wasfound){
            std::vector<cv::Point3f> _3dpoints =  fsiv_generate_3d_calibration_points(board_size, size);
            fsiv_compute_camera_pose(_3dpoints, corner_points, camera_matrix, dist_coeffs, rvec, tvec);

            if(parser.has("i")){
                fsiv_project_image(projected_image, input_frame, board_size, corner_points);
            }

            if(draw_axis){
                fsiv_draw_axes(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size, 5);
            }else{
                fsiv_draw_3d_model(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size);
            }
        }
        //

        cv::namedWindow("VIDEO", cv::WINDOW_GUI_EXPANDED);

        int wait_time = (is_camera ? 20 : 1000.0/25.0); //for a video file we use 25fps.
        int key = 0;

        while (key!=27)
        {            
            //TODO
            if(parser.has("v")){
                cv::Mat projected_frame;
                projected_video >> projected_frame;
                
                fsiv_project_image(projected_frame, input_frame, board_size, corner_points);

                if(draw_axis){
                    fsiv_draw_axes(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size, 5);
                }else{
                    fsiv_draw_3d_model(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size);
                }
            }

            if(!input_frame.empty()){
                std::vector<cv::Point2f> corner_points;
                wasfound = fsiv_fast_find_chessboard_corners(input_frame, board_size, corner_points);
                if(wasfound){
                    std::vector<cv::Point3f> _3dpoints =  fsiv_generate_3d_calibration_points(board_size, size);
                    fsiv_compute_camera_pose(_3dpoints, corner_points, camera_matrix, dist_coeffs, rvec, tvec);

                    if(parser.has("i")){
                    fsiv_project_image(projected_image, input_frame, board_size, corner_points);
                    }

                    if(draw_axis){
                        fsiv_draw_axes(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size, 5);
                    }else{
                        fsiv_draw_3d_model(input_frame, camera_matrix, dist_coeffs, rvec, tvec, size);
                    }
                }
                
                cv::imshow("VIDEO", input_frame);
            }
            

            //
            key = cv::waitKey(wait_time) & 0xff;
            cap >> new_input_frame;
            if(!new_input_frame.empty()){
                input_frame = new_input_frame.clone();
            }
        }
        cv::destroyAllWindows();
    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}
