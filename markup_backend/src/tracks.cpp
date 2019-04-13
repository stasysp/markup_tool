#include "markup_backend/tracks.h"

TrackContainer make_dummy_track() {
    return TrackContainer(5);
}

// ---------------- class Track --------------------------

void Track::push_back(Detection det) {
    assert(det.id == id_);

    detections_.push_back(det);
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
    for (const auto& detection : detections_) {
        if (detection.frame == frame_idx) {
            return std::make_unique<Detection>(detection);
        }
    }
    return nullptr;
}

// ---------------- class TrackContainer -----------------

TrackContainer::TrackContainer(size_t video_length) {
    this->video_len_ = video_length;
    timeline_.resize(this->video_len_);
}

void TrackContainer::add_track(const Track& track) {
    // Checks
    tracks_.insert(track);
    for (auto& saved_track : tracks_) {
        if (saved_track.get_id() == track.get_id()) {
            for (auto &detection : saved_track) {
                assert(detection.frame < video_len_);
                timeline_[detection.frame].insert(&detection);
            }
            break;
        }
    }
}

std::unique_ptr<Track> TrackContainer::get_track(size_t id) const {

    for (const auto& track : tracks_) {
        if (track.get_id() == id) {
            return std::make_unique<Track>(track);
        }
    }

    return nullptr;
}

std::vector<Detection> TrackContainer::get_detections(size_t frame_idx) const {
    assert(frame_idx < video_len_);

    std::vector<Detection> detections;
    for (const auto& detection_ptr : timeline_[frame_idx]) {
        detections.push_back(*detection_ptr);
    }
    return detections;
}

size_t TrackContainer::get_video_len() const {
    return video_len_;
}

size_t TrackContainer::get_num_tracks() const {
    return tracks_.size();
}

bool TrackContainer::empty() const {
    return tracks_.size() == 0;
}
