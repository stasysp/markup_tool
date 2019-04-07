#include <iostream>

#include "markup_backend/nn_model.h"
#include "markup_backend/markup.h"

TrackContainer MarkUp::run(const Video& video) {
    /* std::vector<std::vector<Detection>> video_detections;
    for (const auto& frame : video) {
        std::vector<Detection> frame_detections = detector_.detect(frame);
        video_detections.emplace_back(frame_detections);
    }

    TrackContainer tracks = assign_tracks(video_detections, id_model_);*/
    TrackContainer tracks(5);
    return tracks;
}

