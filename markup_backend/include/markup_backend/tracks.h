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

    Detection* push_back(const Detection& det);
    Detection* add(const Detection& det);
    size_t get_id() const;

    std::list<Detection>::iterator begin();
    std::list<Detection>::iterator end();

    std::list<Detection>::const_iterator begin() const;
    std::list<Detection>::const_iterator end() const;

    std::unique_ptr<Detection> get_detection(size_t frame_idx);
    std::unique_ptr<Detection> get_last_detection() {
        if (detections_.empty()) {
            return nullptr;
        }

        return std::make_unique<Detection>(detections_.back());
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

    TrackContainer(size_t video_length);
    TrackContainer(const std::string& tracks_filepath);

    void add_track(const Track& track);
    bool add_det2track(size_t track_id, const Detection& det);
    bool has_track(size_t id);
    bool delete_track(size_t id);

    std::unique_ptr<Track> get_track(size_t id);

    std::vector<Detection> get_detections(size_t frame_idx) const;

    size_t get_video_len() const;
    size_t get_num_tracks() const;

    bool empty() const;

    bool save(const std::string& filepath);

    bool load(const std::string& filepath);

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
    // std::set<Track, TrackComparator>
    std::list<Track> tracks_;
    // std::vector<std::set<const Detection*, DetectionComparator>> timeline_;
    std::vector<std::list<Detection*>> timeline_;
    size_t pedestrian_class_ = 1;
};

