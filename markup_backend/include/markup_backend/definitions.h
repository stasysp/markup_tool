#pragma once

#include <opencv2/core/core.hpp>

struct Detection {
    size_t frame;
    int id = -1;
    cv::Rect bbox;
    float confidence;
};