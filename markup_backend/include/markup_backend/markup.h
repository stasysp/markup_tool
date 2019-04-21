#pragma once

#include <string>


#include "markup_backend/nn_model.h"
#include "markup_backend/tracks.h"
#include "markup_backend/video.h"

// using namespace boost::filesystem;

struct PipelineRunParams {
    // std::string detector_model_path;
    // std::string id_model_path; // maybe path to python file?
    std::string tracker_model_path;
    std::string tmp_video_dir = "/tmp/markup_video";
    std::string tmp_img_extention = ".png";
    size_t tmp_img_path_pad2length = 10;
    std::string tracks_filename = "tracks.txt";
};

class MarkUp {
public:
    MarkUp() = delete;
    explicit MarkUp(const PipelineRunParams& params) : params_(params) {
        /* : detector_(params.detector_model_path),
            id_model_(params.id_model_path) { */
    }

    TrackContainer run(const Video& video);

    /* json ReadTracks() {
        std::ifstream f("tracks.json");
        json tracks;
        f >> tracks;
        retunr tracks;
    } */
    
private:

    PipelineRunParams params_;

    // Detector detector_;
    // IDModel id_model_;
};
