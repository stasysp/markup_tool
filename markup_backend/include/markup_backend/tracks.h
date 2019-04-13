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

    void push_back(Detection det);
    size_t get_id() const;

    std::list<Detection>::iterator begin();
    std::list<Detection>::iterator end();

    std::list<Detection>::const_iterator begin() const;
    std::list<Detection>::const_iterator end() const;

    std::unique_ptr<Detection> get_detection(size_t frame_idx) ;

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

    explicit TrackContainer(size_t video_length);

    void add_track(const Track& track);

    std::unique_ptr<Track> get_track(size_t id) const;

    std::vector<Detection> get_detections(size_t frame_idx) const;

    size_t get_video_len() const;
    size_t get_num_tracks() const;

    bool empty() const;

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

TrackContainer make_dummy_track();
