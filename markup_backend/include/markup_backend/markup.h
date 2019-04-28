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
    std::string weights_path = "";
    std::string tmp_video_dir = "/tmp/markup_video";
    std::string tracks_path = "";
    std::string tmp_img_extention = ".png";
    size_t tmp_img_path_pad2length = 10;
    std::string tracks_filename = "tracks.txt";
};

class MarkUp {
public:
    bool get_frame(size_t frame_idx,
                   std::vector<Detection>* detections);

    bool set_model(const std::string& model_path,
                   const std::string& weights_path);

    bool set_video(const std::string& filepath);

    bool set_tracks(const std::string& filepath);

    bool run();

    bool split_track(size_t track_id, size_t frame2split_idx) {
        return false;
    }

    bool unite_tracks(size_t track_id_one, size_t track_id_two);

    bool delete_track(size_t id);

    bool add_detection(const Detection& det);

    bool delete_detection(size_t track_id, size_t frame_idx);

    size_t get_video_len() const;

    PipelineRunParams get_params() const;

    bool load_markup(const std::string& path2file);

    bool save_markup(const std::string& path2file);

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
