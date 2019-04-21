#include "opencv2/highgui/highgui.hpp"
#include "boost/filesystem.hpp"

#include "markup_backend/video.h"

#include <iostream>

using namespace boost::filesystem;

Video::Video(const std::string& dir_path, size_t fps) {
    dir_path_ = dir_path;

    assert(exists(dir_path));

    for (directory_iterator it(dir_path); it!=directory_iterator(); ++it) {
        if (is_regular_file(it->status())) {
            sequence_.push_back(it->path().filename().string());
        }
    }

    std::sort(sequence_.begin(), sequence_.end());
}

cv::Mat Video::get_frame(size_t index) const {
    // TODO: Add exceptions
    assert(index < this->size());

    cv::Mat img = cv::imread((path(dir_path_) / path(sequence_[index])).string());

    return img;
}

