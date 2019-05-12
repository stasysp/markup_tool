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
            break;
        }
    }
}

bool Track::has(size_t frame_idx) const {
    for (const auto& det : detections_) {
        if (det.frame == frame_idx) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Detection> Track::get_last_detection() {
    if (detections_.empty()) {
        return nullptr;
    }

    return std::make_unique<Detection>(detections_.back());
}

std::unique_ptr<Detection> Track::get_first_detection() {
    if (detections_.empty()) {
        return nullptr;
    }

    return std::make_unique<Detection>(detections_.front());
}

Detection* Track::add(const Detection& det) {
    assert(det.id == id_);

    std::list<Detection>::iterator det_iter;
    for (det_iter = detections_.begin();
             det_iter != detections_.end();
             ++det_iter) {

        if (det_iter->frame >= det.frame) {
            break;
        }
    }

    if (det_iter != detections_.end() && det_iter->frame == det.frame) {
        *det_iter = det;
        return &(*det_iter);
    } else {
        std::list<Detection>::iterator new_det = detections_.insert(det_iter, det);
        return &(*new_det);
    }
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

DetectionAndTrack Track::make_detection_track(size_t frame_idx,
                                       size_t max_frames_before,
                                       size_t max_frames_after) {
    DetectionAndTrack det_n_track;

    det_n_track.track_id = this->get_id();
    assert(this->has(frame_idx));

    auto current_det = this->get_detection(frame_idx);
    assert(current_det != nullptr);

    det_n_track.frame_idx = frame_idx;

    int min_frame = int(frame_idx) - int(max_frames_before);
    int max_frame = int(frame_idx) + int(max_frames_after);

    for (auto det : this->detections_) {
        if (min_frame <= int(det.frame) && int(det.frame) <= max_frame) {
            det_n_track.track_frames.push_back(det.frame);
            det_n_track.track.emplace_back(cv::Point2i(det.bbox.x + det.bbox.width / 2, det.bbox.y + det.bbox.height / 2));
        }
    }

    assert(!det_n_track.track_frames.empty());
    assert(!det_n_track.track.empty());

    det_n_track.bbox = current_det->bbox;

    return det_n_track;
}
