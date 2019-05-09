#pragma once

#include <list>
#include <memory>
#include <map>

#include <iostream>

#include "markup_backend/definitions.h"
#include "markup_backend/tracks.h"

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
    bool delete_detection(size_t track_id, size_t frame_idx);
    bool unite_tracks(size_t id_A, size_t id_B);
    bool split_track(size_t track_id, size_t frame_idx);
    size_t create_new_track(const Detection& det);
    std::vector<DetectionAndTrack> get_tracks_and_detections(size_t frame_idx_,
                                                             size_t max_frames_before,
                                                             size_t max_frames_after);

    std::unique_ptr<Track> get_track(size_t id);

    std::vector<Detection> get_detections(size_t frame_idx) const;

    std::map<size_t, std::vector<Detection>> get_slice(size_t min_frame_idx, size_t max_frame_idx) const;

    size_t get_video_len() const;
    size_t get_num_tracks() const;

    bool empty() const;

    bool save(const std::string& filepath);

    bool load(const std::string& filepath);

    // Split tracks
    // Re-id tracks
private:
    size_t get_new_id();
    size_t video_len_;
    // std::set<Track, TrackComparator>
    std::list<Track> tracks_;
    // std::vector<std::set<const Detection*, DetectionComparator>> timeline_;
    std::vector<std::list<Detection*>> timeline_;
    size_t pedestrian_class_ = 1;
};
