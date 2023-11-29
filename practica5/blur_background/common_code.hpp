#pragma once

#include <opencv2/core/core.hpp>

/**
 * @brief Compute the dense optical flow using the Farneback method.
 * @see https://docs.opencv.org/3.4.8/de/d9e/classcv_1_1FarnebackOpticalFlow.html
 * @param[in] prev is the previous frame.
 * @param[in] next is the next frame.
 * @param[in,out] flow is the dense optical flow. If not is empty, it will be used as initial guess.
 * @pre prev.type()==CV_8UC1
 * @pre next.type()==CV_8UC1
 * @pre flow.empty() || flow.type()==CV_32FC2
 * @post flow.type()==CV_32FC2
 */
void fsiv_compute_dense_optical_flow(cv::Mat const& prev,
                                     cv::Mat const& next,
                                     cv::Mat & flow);

/**
 * @brief Compute the magnitude of the optical flow.
 * @param[in] flow is the dense opctical flow.
 * @param mag if the flow magnitude.
 * @pre flow.type()==CV_32FC2
 * @post mag.type()==CV_32FC1
 */
void fsiv_compute_optical_flow_magnitude(cv::Mat &flow, cv::Mat& mag);


/**
 * @brief Create a morphological structure element.
 * 
 * @param ste_r is the radius of the ste. 
 * @param ste_type is the type. @see opencv docs.
 * @return the ste.
 */
cv::Mat fsiv_create_structuring_element(int ste_r, int ste_type);


/**
 * @brief Compute a foreground mask using the magnitude of dense optical flow as feature. 
 * @param[in] prev is the prev frame.
 * @param[in] curr is the current frame.
 * @param[in,out] flow as input it is the previous computed optical flow. It is used as initial estimation. Use an empty mat if you don't want use initial estimation. As output, it saves the computed optical flow.
 * @param[in,out] mask as input, it is the old mask and, as output, the updated mask. 
 * @param[in] t is the optical flow magnitude threshold to consider a pixel as foreground.
 * @param[in] ste_r do a dilation using this a morphological structure element. Default value 0 is do nothing.
 * @param[in] ste_type use this type of ste. Default value is a ellipse.
 * @param[in] alpha is the memory factor. Default is to remember nothing.
 * @pre !mag.empty() && mag.type()==CV_32FC1
 * @pre mask.empty() || (mask.size()=mag.size() && mask.type()==CV_8UC1)
 */
void fsiv_compute_of_foreground_mask(cv::Mat const& prev, cv::Mat const& curr,
                                    cv::Mat& flow,
                                     cv::Mat & mask,
                                     const double t,
                                     const int ste_r = 0,
                                     const int ste_type = cv::MORPH_ELLIPSE,
                                     const float alpha = 0.0);

/**
 * @brief Blur the background of a frame given the foreground mask.
 * @param[in] frame is the input frame.
 * @param[in] foreground_mask is the foreground mask. It's assumed in range [0.0, 1.0].
 * @param[out] bblur_frame is the output frame with the background blurred.
 * @param[in] r is the blur radius.
 * @param[in] gaussian if its true used gaussian blur, else uniform blur.
 * @pre frame.type()==CV_32FC3
 * @pre foreground_mask.type()==CV_32FC3
 * @pre r>0
 * @post bblur_frame.type()==CV_32FC3
 */
void fsiv_blur_background(cv::Mat const& frame,
                          cv::Mat const& foreground_mask,
                          cv::Mat & bblur_frame,
                          const int r=11,
                          const bool gaussian=false);


