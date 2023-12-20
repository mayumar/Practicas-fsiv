#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

/**
 * @brief Load the dataset into memory.
 * 
 * @param folder the pathname where the dataset files were downloaded.
 * @param partition which partition to load, 0:train, 1:test
 * @param X are the images (on row by image)
 * @param y are the labels.
 * @pre partition==0 || partition==1
 * @post X.rows==y.rows
 * @post X.type()=CV_8UC1
 * @post y.type()=CV_32SC1
 */
void fsiv_load_dataset(std::string& folder, int partition, cv::Mat& X, cv::Mat& y);

/**
 * @brief Get the description of a given class label.
 * 
 * @param id is the class label.
 * @return the description.
 * @pre 0<=id<10
 */
const std::string& fsiv_get_dataset_label_name(int id);

/**
 * @brief Split a dataset into train/validation partitions.
 * 
 * @param val_percent is the percentage used to validation.
 * @param X are the images (one row by image.)
 * @param y are the class labels.
 * @param X_t is the train images partition.
 * @param y_t is the train labels partition.
 * @param X_v is the validation images partition.
 * @param y_v is the validation labels partition.
 * @pre 0.0<=val_percent && val_percent<1.0
 * @post X.rows == X_t.rows+X_V.rows
 * @post y.rows == y_t.rows+y_v.rows
 */
void fsiv_split_dataset(float val_percent, 
                        const cv::Mat& ds_images,
                        const cv::Mat& ds_labels,
                        cv::Mat& train_images, cv::Mat& train_labels,
                        cv::Mat& validate_images, cv::Mat& validate_labels);


/**
 * @brief Random subsample a dataset without replacement.
 * 
 * @param X dataset features.
 * @param y dataset labels.
 * @param X_s subsampled samples.
 * @param y_s subsampled labels.
 * @param p subsample size ratio. Default 0.5;
 */
void fsiv_subsample_dataset(const cv::Mat& X, const cv::Mat& y,
    cv::Mat& X_s, cv::Mat& y_s, float p=0.5);

/**
 * @brief Compute the size in bytes of a file.
 * 
 * @param[in] path is the pathname of the file.
 * @param[out] size is the size of the file in bytes.
 * @return true if success.
 */
bool fsiv_compute_file_size(std::string const& path, size_t& size);
