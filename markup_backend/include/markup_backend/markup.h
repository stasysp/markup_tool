#pragma once

#include <string>

#include "markup_backend/nn_model.h"
#include "markup_backend/tracks.h"
#include "markup_backend/video.h"
#include "markup_backend/utils.h"

#include <boost/filesystem.hpp>
// using namespace boost::filesystem;

struct PipelineRunParams {
    // std::string detector_model_path;
    // std::string id_model_path; // maybe path to python file?
    std::string video_path = "";
    std::string tracker_model_path = "";
    std::string tmp_video_dir = "/tmp/markup_video";
    std::string debug_gt_tacks = "../markup_tool/data/test/MOT16-04/gt/gt.txt";
    std::string tmp_img_extention = ".png";
    size_t tmp_img_path_pad2length = 10;
    std::string tracks_filename = "tracks.txt";
};

class MarkUp {
public:
    bool get_frame(size_t frame_idx, std::vector<Detection>* detections) {
        if (detections == nullptr) {
            // TODO: Exceptions
            std::cout << "No return container 'detections' provided!" << std::endl;
            return false;
        }

        if (track_container_ == nullptr) {
            // TODO: Exceptions
            std::cout << "First compute tracks!" << std::endl;
            return false;
        }

        if (frame_idx >= track_container_->get_video_len()) {
            // TODO: Exceptions
            std::cout << "No such frame:" << frame_idx
                      << " Only has " << track_container_->get_video_len() << std::endl;
            return false;
        }

        *detections = track_container_->get_detections(frame_idx);

        return true;
    }

    bool set_video(const std::string& filepath) {
        if (filepath.empty()) {
            return false;
        }

        if (!boost::filesystem::exists(filepath)) {
            // TODO: Exceptions
            std::cout << "File not found:" << filepath << std::endl;
            return false;
        }

        if (params_.video_path == filepath) {
            return true;
        }

        // Clear state
        video_.reset(nullptr);
        track_container_.reset(nullptr);

        params_.video_path = filepath;
        video_ = std::make_unique<Video>(params_.video_path);
    }

    bool run() {
        track_container_.reset(nullptr);

        if (video_ == nullptr) {
            // TODO: Exceptions
            std::cout << "Set Video first!" << std::endl;
            return false;
        }

        // track_container_ = this->run_pipeline(*video_);

        /* if (boost::filesystem::exists(params_.debug_gt_tacks)) {
            // TODO: Exceptions
            std::cout << "No debug tracks:" << params_.debug_gt_tacks << std::endl;
            return false;
        }*/

        track_container_ = read_dummy_trackcontainer(params_.debug_gt_tacks);

        return true;
    }

    bool split_track(size_t track_idx, size_t frame2split_idx) {
        return true;
    }
    bool unite_tracks(int track_idx_one, int track_idx_two) {
        return true;
    }
    bool add_bbox() {
        return true;
    }
    bool detete_bbox(size_t track_idx, size_t frame_idx) {
        return true;
    }

private:
    PipelineRunParams params_;
    std::unique_ptr<Video> video_ = nullptr;
    std::unique_ptr<TrackContainer> track_container_ = nullptr;

    std::unique_ptr<TrackContainer> run_pipeline(const Video& video);
};


/*class MarkUp {
public:
    MarkUp() = delete;
    explicit MarkUp(const PipelineRunParams& params) : params_(params) {
    }

    std::unique_ptr<TrackContainer> run(const Video& video);
    
private:

    PipelineRunParams params_;

    // Detector detector_;
    // IDModel id_model_;
};*/
