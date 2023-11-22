#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include "common_code.hpp"

std::vector<cv::Point3f>
fsiv_generate_3d_calibration_points(const cv::Size& board_size,
                                    float square_size)
{
    std::vector<cv::Point3f> ret_v;
    //TODO
    //Remember: the first inner point has (1,1) in board coordinates.

    //
    CV_Assert(ret_v.size() == 
        static_cast<size_t>(board_size.width*board_size.height));
    return ret_v;
}


bool
fsiv_fast_find_chessboard_corners(const cv::Mat& img, const cv::Size &board_size,
                             std::vector<cv::Point2f>& corner_points)
{
    CV_Assert(img.type()==CV_8UC3);
    bool was_found = false;
    // TODO
    // Hint: use cv::findChessboardCorners adding fast check to the defaults flags.

    //
    return was_found;
}


void fsiv_compute_camera_pose(const std::vector<cv::Point3f> &_3dpoints,
                              const std::vector<cv::Point2f> &_2dpoints,
                              const cv::Mat& camera_matrix,
                              const cv::Mat& dist_coeffs,
                              cv::Mat& rvec,
                              cv::Mat& tvec)
{
    CV_Assert(_3dpoints.size()>=4 && _3dpoints.size()==_2dpoints.size());
    // TODO
    // Hint: use cv::solvePnP to the pose of a calibrated camera.

    //
    CV_Assert(rvec.rows==3 && rvec.cols==1 && rvec.type()==CV_64FC1);
    CV_Assert(tvec.rows==3 && tvec.cols==1 && tvec.type()==CV_64FC1);
}

void
fsiv_draw_axes(cv::Mat& img,
               const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs,
               const cv::Mat& rvec, const cv::Mat& tvec,
               const float size, const int line_width)
{
    // TODO
    // Hint: use cv::projectPoints to get the image coordinates of the 3D points
    // (0,0,0), (size, 0, 0), (0, size, 0) and (0, 0, -size) and draw a line for
    // each axis: blue for axis OX, green for axis OY and red for axis OZ.
    // Warning: use of cv::drawFrameAxes() is not allowed.

    //
}

void
fsiv_load_calibration_parameters(cv::FileStorage &fs,
                                 cv::Size &camera_size,
                                 float& error,
                                 cv::Mat& camera_matrix,
                                 cv::Mat& dist_coeffs,
                                 cv::Mat& rvec,
                                 cv::Mat& tvec)
{
    CV_Assert(fs.isOpened());
    //TODO
    // Hint: use fs["label"] >> var to load data items from the file.
    // @see cv::FileStorage operators "[]" and ">>"


    //
    CV_Assert(fs.isOpened());
    CV_Assert(camera_matrix.type()==CV_64FC1 && camera_matrix.rows==3 && camera_matrix.cols==3);
    CV_Assert(dist_coeffs.type()==CV_64FC1 && dist_coeffs.rows==1 && dist_coeffs.cols==5);
    CV_Assert(rvec.type()==CV_64FC1 && rvec.rows==3 && rvec.cols==1);
    CV_Assert(tvec.type()==CV_64FC1 && tvec.rows==3 && tvec.cols==1);
    return;
}

void
fsiv_draw_3d_model(cv::Mat &img, const cv::Mat& M, const cv::Mat& dist_coeffs,
                   const cv::Mat& rvec, const cv::Mat& tvec,
                   const float size)
{
    CV_Assert(img.type()==CV_8UC3);

    // TODO
    // Hint: build a 3D object points vector with pair of segments end points.
    // Use cv::projectPoints to get the 2D image coordinates of 3D object points,
    // build a vector of vectors of Points, one for each segment, and use
    // cv::polylines to draw the wire frame projected model.

    //
}

void
fsiv_project_image(const cv::Mat& input, cv::Mat& output,
                   const cv::Size& board_size,
                   const std::vector<cv::Point2f>& chess_board_corners)
{
    CV_Assert(!input.empty() && input.type()==CV_8UC3);
    CV_Assert(!output.empty() && output.type()==CV_8UC3);
    CV_Assert(static_cast<size_t>(board_size.area()) == 
        chess_board_corners.size());
    
    // TODO
    // Hint: get the upper-left, upper-right, bottom-right and bottom-left 
    //   chess_board_corners and map to the upper-left, upper-right, bottom-right 
    //   and bottom-left input image point coordinates. 
    //   Use cv::getPerspectiveTransform compute such mapping.
    // Hint: use cv::wrapPerspective to get a wrap version of the input image
    //   using the computed mapping. Use INTER_LINEAR as interpolation method 
    //   and use BORDER_TRANSPARENT as a border extrapolation method 
    //   to maintain the underlying image.
    // 

    //
}
