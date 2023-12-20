#include <iostream>
#include <exception>
#include <fstream>
#include <opencv2/imgproc.hpp>

#include "dataset.hpp"

#include "mnist_reader.hpp"

static cv::Mat 
to_Mat(std::vector<std::vector<std::uint8_t>> const & images)
{
  cv::Mat mat = cv::Mat(images.size(), 784 /*28x28*/,  CV_8UC1);
  for (size_t i=0; i<images.size(); i++)
    std::copy(images[i].begin(), images[i].end(), mat.row(i).begin<uint8_t>());
  return mat;
}

static cv::Mat 
to_Mat(std::vector<std::uint8_t> const & labels)
{
  cv::Mat mat = cv::Mat(labels.size(), 1,  CV_8UC1);  
  std::copy(labels.begin(), labels.end(), mat.begin<uint8_t>());
  return mat;
}

void
fsiv_load_dataset(std::string& folder, int partition,
                    cv::Mat& X, cv::Mat& y)
{
    CV_Assert(partition==0 || partition==1);
    int train_limit = 0;
    int test_limit = -1;
    if (partition != 0) 
    {
        train_limit = -1;
        test_limit = 0;
    }
    auto dataset = mnist::read_dataset<std::vector, std::vector,
            std::uint8_t, std::uint8_t>(folder, train_limit, test_limit);
    if (partition==0)
    {
        X = to_Mat(dataset.training_images);
        y = cv::Mat_<std::int32_t>(to_Mat(dataset.training_labels));
    }
    else
    {
        X = to_Mat(dataset.test_images);
        y = cv::Mat_<std::int32_t>(to_Mat(dataset.test_labels));
    }
    CV_Assert(X.rows == y.rows);
    CV_Assert(X.type()==CV_8UC1);
    CV_Assert(y.type()==CV_32SC1);
}

static std::string fsiv_fashion_mnist_label_names [] = {
        "T-shirt/top", "Trouser", "Pullover",
        "Dress", "Coat", "Sandal", "Shirt", "Sneaker",
        "Bag", "Ankle boot"
    };

const std::string&
fsiv_get_dataset_label_name(int id)
{
    CV_Assert(0<=id && id<10);
    return fsiv_fashion_mnist_label_names[id];
}

void
fsiv_split_dataset(float val_percent, const cv::Mat& X,
                                 const cv::Mat& y,
                                 cv::Mat& X_t, cv::Mat& y_t,
                                 cv::Mat& X_v, cv::Mat& y_v)
{
    CV_Assert(0.0<=val_percent && val_percent<1.0);
    int train_size = X.rows * (1.0 - val_percent);
    X_t = X.rowRange(0, train_size);
    y_t = y.rowRange(0, train_size);
    X_v = X.rowRange(train_size, X.rows);
    y_v = y.rowRange(train_size, y.rows);
    CV_Assert(X.rows == (X_t.rows+X_v.rows));
    CV_Assert(y.rows == (y_t.rows+y_v.rows));
}

bool 
fsiv_compute_file_size(std::string const& path, size_t& size)
{
    bool success = true;
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary | std::ios::ate);

    if (file)
        size = file.tellg();
    else
        success = false;

    return success;
}

void 
fsiv_subsample_dataset(const cv::Mat& X, const cv::Mat& y,
    cv::Mat& X_s, cv::Mat& y_s, float p)
{
    CV_Assert(X.rows == y.rows);
    CV_Assert(p>0.0 && p<=1.0f);
    const int subsample_size = X.rows * p;
    cv::Mat idxs = cv::Mat(X.rows, 1, CV_32SC1);
    for (int i=0; i<X.rows; ++i)
        idxs.at<int>(i) = i;
    cv::randShuffle(idxs);
    X_s = cv::Mat(subsample_size, X.cols, X.type());
    y_s = cv::Mat(subsample_size, y.cols, y.type());

#ifdef USE_OPENMP
#pragma openmp parallel for
#endif    
    for (int i=0; i<subsample_size; ++i)
    {
        const int idx=idxs.at<int>(i);
        X.row(idx).copyTo(X_s.row(i));
        y.row(idx).copyTo(y_s.row(i));
    }
}
