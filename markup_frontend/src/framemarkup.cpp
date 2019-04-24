#include "markup_frontend/framemarkup.h"

FrameMarkup::FrameMarkup(){}

QMap<int, ScaledBBox>::const_iterator FrameMarkup::begin() const {
    return bboxes.begin();
}

QMap<int, ScaledBBox>::const_iterator FrameMarkup::end() const {
    return bboxes.end();
}

void FrameMarkup::add(int track_id, ScaledBBox bbox) {
    bboxes[track_id] = std::move(bbox);
}

void FrameMarkup::remove(int track_id) {
    auto iter = bboxes.find(track_id);
    if (iter != bboxes.end()) {
        bboxes.erase(iter);
    }
}
