#pragma once

#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

#include "markup_backend/definitions.h"


class NNModel {
public:
    NNModel() = delete;
    explicit NNModel(const std::string& path) {}
private:
    void load_model();
    // model
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