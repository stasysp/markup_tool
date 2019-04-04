#pragma once

#include <string>
#include <vector>

#include <opencv2/core/core.hpp>



class NNModel {
public:
    NNModel() = delete;
    explicit NNModel(const std::string& path) {}
private:
    void load_model();
    // model
};

struct Detection {
    size_t frame;
    int id = -1;
    cv::Rect bbox;
    float confidence;
};

class Detector : public NNModel {
public:
    explicit Detector(const std::string& path) : NNModel(path) {}
    std::vector<Detection> detect(const cv::Mat& img);
private:
    // preprocess_input();
};

class IDModel : public NNModel {
public:
    explicit IDModel(const std::string& path) : NNModel(path) {}
    float score(const cv::Mat& detection_A, const cv::Mat& detection_B);
private:
    // preprocess_input();
};