#pragma once

#include <opencv2/core/core.hpp>

struct Detection {
    size_t frame;
    int id = -1;
    cv::Rect bbox;
    float confidence;
};

struct DetectionAndTrack {
    size_t track_id;
    cv::Rect bbox;
    std::vector<cv::Point2i> track;
    std::vector<size_t> track_frames;
    size_t frame_idx;

    size_t get_first_frame_idx() const {
        assert(!track_frames.empty());

        return track_frames.front();
    }

    int get_last_frame_idx() const {
        assert(!track_frames.empty());

        return track_frames.back();
    }
};

