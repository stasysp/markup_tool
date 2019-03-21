#pragma once

#include "opencv2/core.hpp"

class Video {
public:
    Video() = delete;
    explicit Video(const std::string& path);
    // iterators begin end
    // get_frame(index)
private:
    std::vector<cv::Mat> sequence;
    size_t fps;
};
