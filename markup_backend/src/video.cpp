#include "opencv2/highgui/highgui.hpp"
#include "boost/filesystem.hpp"

#include "markup_backend/video.h"

using namespace boost::filesystem;

Video::Video(const std::string& dir_path) {
    exists(dir_path);

    for (directory_iterator it(dir_path); it!=directory_iterator(); ++it) {
        if (is_regular_file(it->status())) {
            sequence.push_back(it->path().filename().string());
        }
    }

    std::sort(sequence.begin(), sequence.end());
}

cv::Mat Video::get_frame(size_t index) const {
    if (index >= this->size()) {
        return cv::Mat();
    }

    cv::Mat img = cv::imread(sequence[index]);
    return img;
}

