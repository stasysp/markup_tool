#include <fstream>

#include "markup_backend/tracks.h"



// ---------------- class Track --------------------------

Detection* Track::push_back(const Detection& det) {
    assert(det.id == id_);

    detections_.push_back(det);

    return &detections_.back();
}

void Track::del(size_t frame_idx) {
    for (auto it = this->detections_.begin();
             it != this->detections_.end();
             ++it) {
        if (it->frame == frame_idx) {
            this->detections_.erase(it);
        }
    }
}

Detection* Track::add(const Detection& det) {
    assert(det.id == id_);

    std::list<Detection>::iterator det_iter;
    for (det_iter = detections_.begin();
             det_iter != detections_.end();
             ++det_iter) {
        if (det_iter->frame > det.frame) {
            break;
        }
    }
    std::list<Detection>::iterator new_det = detections_.insert(det_iter, det);
    return &(*new_det);
}

size_t Track::get_id() const {
    return id_;
}

std::list<Detection>::iterator Track::begin() {
    return detections_.begin();
}

std::list<Detection>::iterator Track::end() {
    return detections_.end();
}

std::list<Detection>::const_iterator Track::begin() const {
    return detections_.begin();
}

std::list<Detection>::const_iterator Track::end() const {
    return detections_.end();
}

std::unique_ptr<Detection> Track::get_detection(size_t frame_idx) {
    for (auto& detection : detections_) {
        if (detection.frame == frame_idx) {
            return std::make_unique<Detection>(detection);
        }
    }
    return nullptr;
}

