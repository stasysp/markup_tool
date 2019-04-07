#pragma once

#include <list>
#include <memory>
#include <set>

#include <iostream>

#include "markup_backend/definitions.h"

class Track {
public:
    Track() = delete;
    explicit Track(size_t id) : id_(id) {}
    void push_back(Detection det) {
        assert(det.id == id_);

        detections_.push_back(det);
    }

    size_t get_id() const {
        return id_;
    }

    std::list<Detection>::iterator begin() {
        return detections_.begin();
    }

    std::list<Detection>::iterator end() {
        return detections_.end();
    }

    std::list<Detection>::const_iterator begin() const {
        return detections_.begin();
    }

    std::list<Detection>::const_iterator end() const {
        return detections_.end();
    }

    std::unique_ptr<Detection> get_detection(size_t frame_idx) {
        for (const auto& detection : detections_) {
            if (detection.frame == frame_idx) {
                return std::make_unique<Detection>(detection);
            }
        }
        return nullptr;
    }

private:
    std::list<Detection> detections_;
    size_t id_;
};

struct TrackComparator {
    bool operator() (const Track& lhs, const Track& rhs) const {
        return lhs.get_id() < rhs.get_id();
    }
};

struct DetectionComparator {
    bool operator() (const Detection* const lhs, const Detection* const rhs) const {
        return lhs->id < rhs->id;
    }
};

class TrackContainer {
public:
    TrackContainer() = delete;

    // TrackContainer() = default;
    explicit TrackContainer(size_t video_length) {
        this->video_len_ = video_length;
        timeline_.resize(this->video_len_);
    }

    void add_track(const Track& track) {
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

    std::unique_ptr<Track> get_track(size_t id) const {
        for (const auto& track : tracks_) {
            if (track.get_id() == id) {
                return std::make_unique<Track>(track);
            }
        }

        return nullptr;
    }

    std::vector<Detection> get_detections(size_t frame_idx) const {
        assert(frame_idx < video_len_);

        std::vector<Detection> detections;
        for (const auto& detection_ptr : timeline_[frame_idx]) {
            detections.push_back(*detection_ptr);
        }
        return detections;
    }

    size_t get_video_len() const {
        return video_len_;
    }

    size_t get_num_tracks() const {
        return tracks_.size();
    }

    bool empty() const {
        return tracks_.size() == 0;
    }

    // void add_track(Track);
    // get all detections at frame_index
    // iterate through all tracks
    // Get track by id
    // Merge tracks
    // Split tracks
    // Delete tracks
    // Re-id tracks
private:
    // std::vector<std::vector<>>
    // tracks
    size_t video_len_;
    std::set<Track, TrackComparator> tracks_;
    std::vector<std::set<const Detection*, DetectionComparator>> timeline_;
};

/*class DummyTrackContainer {
public:
    // TrackContainer() = delete;
    DummyTrackContainer() {

    }

    void add_track(const Track& track) {

    }

    std::unique_ptr<Track> get_track(size_t id) const {
        for (const auto& track : tracks_) {
            if (track.get_id() == id) {
                return std::make_unique<Track>(track);
            }
        }

        return nullptr;
    }

    //std::vector<Detection> get_detections(size_t frame_idx) {
    //    assert(frame_idx < video_len_);
    // }

    size_t get_video_len() const {
        return video_len_;
    }
private:
    size_t video_len_;
    std::vector<Track> tracks_;
};*/



TrackContainer make_dummy_track();
