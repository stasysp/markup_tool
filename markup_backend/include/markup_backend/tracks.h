#pragma once

#include <list>
#include <memory>

#include "markup_backend/definitions.h"

class TrackContainer {
public:
    // TrackContainer() = delete;
    TrackContainer() = default;
    explicit TrackContainer(size_t video_length);
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
};

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

class DummyTrackContainer {
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

    std::vector<Detection> get_detections(size_t frame_idx) {
        assert(frame_idx < video_len_);
    }

    size_t get_video_len() const {
        return video_len_;
    }
private:
    size_t video_len_;
    std::vector<Track> tracks_;
};



DummyTrackContainer make_dummy_track();
