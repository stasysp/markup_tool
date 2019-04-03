#pragma once

#include <string>

#include "markup_backend/nn_model.h"
#include "markup_backend/tracks.h"
#include "markup_backend/video.h"

struct PipelineRunParams {
    std::string detector_model_path;
    std::string id_model_path;
};

class MarkUp {
public:
    MarkUp() = delete;
    explicit MarkUp(const PipelineRunParams& params) :
        detector_(params.detector_model_path),
        id_model_(params.id_model_path) {

    }

    TrackContainer run(const Video& video);

private:
    Detector detector_;
    IDModel id_model_;
};