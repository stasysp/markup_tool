#include <fstream>

#include "markup_backend/track_container.h"

// ---------------- class TrackContainer -----------------

TrackContainer::TrackContainer(size_t video_length) {
    this->video_len_ = video_length;
    timeline_.resize(this->video_len_);
}

TrackContainer::TrackContainer(const std::string& tracks_filepath) {
    assert(this->load(tracks_filepath));
}

size_t TrackContainer::get_new_id() {
    size_t max_id = 0;
    for (const auto& track : this->tracks_) {
        max_id = std::max(track.get_id(), max_id);
    }

    return max_id + 1;
}

size_t TrackContainer::create_new_track(const Detection& det) {
    size_t new_id = this->get_new_id();
    Detection new_det = det;
    new_det.id = new_id;

    Track new_track(new_id);
    new_track.add(new_det);

    this->add_track(new_track);

    return new_id;
}

void TrackContainer::add_track(const Track& track) {
    assert(!this->has_track(track.get_id()));

    // Checks
    tracks_.push_back(track);
    for (std::list<Detection>::iterator it = tracks_.back().begin();
            it != tracks_.back().end(); ++it) {
        assert(it->frame < video_len_);
        assert(timeline_.size() == video_len_);
        timeline_[it->frame].push_back(&(*it));
    }

    /*for (auto& saved_track : tracks_) {
        if (saved_track.get_id() == track.get_id()) {
            for (auto &detection : saved_track) {
                assert(detection.frame < video_len_);
                timeline_[detection.frame].insert(&detection);
            }
            break;
        }
    }*/
}

std::unique_ptr<Track> TrackContainer::get_track(size_t id) {
    for (auto track_iter = tracks_.begin(); track_iter != tracks_.end(); ++track_iter) {
        if (track_iter->get_id() == id) {
            return std::make_unique<Track>(*track_iter);
        }
    }

    return nullptr;
}

bool TrackContainer::has_track(size_t id) {
    for (auto track_iter = tracks_.begin(); track_iter != tracks_.end(); ++track_iter) {
        if (track_iter->get_id() == id) {
            return true;
        }
    }
    return false;
}

bool TrackContainer::interpolate_track(size_t track_id, size_t from_frame_idx, size_t to_frame_idx) {
    assert(from_frame_idx <= to_frame_idx);

    auto track = this->get_track(track_id);

    if (track == nullptr) {
        return false;
    }

    auto from_det = track->get_detection(from_frame_idx);
    if (from_det == nullptr) {
        return false;
    }

    auto to_det = track->get_detection(to_frame_idx);
    if (to_det == nullptr) {
        return false;
    }

    assert(from_det->frame <= to_det->frame);


    for (size_t frame_idx = from_frame_idx + 1; frame_idx < to_frame_idx; ++frame_idx) {
        float alpha = float(frame_idx - from_frame_idx) / float(to_frame_idx - from_frame_idx + 1);
        Detection new_det;

        new_det.id = track_id;
        new_det.frame = frame_idx;
        new_det.bbox.x = int((1 - alpha) * from_det->bbox.x + alpha * to_det->bbox.x);
        new_det.bbox.y = int((1 - alpha) * from_det->bbox.y + alpha * to_det->bbox.y);
        new_det.bbox.width = int((1 - alpha) * from_det->bbox.width + alpha * to_det->bbox.width);
        new_det.bbox.height = int((1 - alpha) * from_det->bbox.height + alpha * to_det->bbox.height);
        new_det.confidence = (1 - alpha) * from_det->confidence + alpha * to_det->confidence;

        this->add_det2track(track_id, new_det);
    }

    return true;
}

bool TrackContainer::split_track(size_t track_id, size_t frame_idx) {
    std::list<Track>::iterator track_it;
    for (track_it = this->tracks_.begin();
             track_it != this->tracks_.end();
             ++track_it) {
        if (track_it->get_id() == track_id) {
            break;
        }
    }

    if (track_it == this->tracks_.end()) {
        return false;
    }

    size_t min_frame = (*track_it).get_first_detection()->frame;
    size_t max_frame = (*track_it).get_last_detection()->frame;

    if (frame_idx <= min_frame || frame_idx > max_frame) {
        return false;
    }

    size_t new_id = get_new_id();
    Track track_head((*track_it).get_id());
    Track track_tail(new_id);

    for (const auto& det : *track_it) {
        if (det.frame < frame_idx) {
            Detection new_det = det;
            new_det.id = track_head.get_id();
            track_head.push_back(new_det);
        } else {
            Detection new_det = det;
            new_det.id = track_tail.get_id();
            track_tail.push_back(new_det);
        }
    }

    this->delete_track((*track_it).get_id());
    this->add_track(track_head);
    this->add_track(track_tail);

    /*std::vector<size_t> frames2delete;
    for (const auto& det : *track_it) {
        if (det.frame >= frame_idx) {
            Detection new_det = det;
            new_det.id = new_id;
            track_tail.push_back(new_det);
            frames2delete.push_back(det.frame);
        }
    }

    if (frames2delete.empty()) {
        return true;
    }

    for (const auto& index : frames2delete) {
        this->delete_detection(track_id, index);
    }*/



    return true;
}

bool TrackContainer::delete_track(size_t id) {
    for (auto track_iter = tracks_.begin(); track_iter != tracks_.end(); ++track_iter) {
        if (track_iter->get_id() == id) {
            // Find the track

            // Delete fast
            for (const auto& det_iter : *track_iter) {
                for (auto frame_ptr_it = timeline_[det_iter.frame].begin();
                         frame_ptr_it != timeline_[det_iter.frame].end();
                         ++frame_ptr_it) {
                    if (id == (*frame_ptr_it)->id) {
                        timeline_[det_iter.frame].erase(frame_ptr_it);
                        break;
                    }

                }
            }

            // Delete the track;
            tracks_.erase(track_iter);
            return true;
        }
    }

    return false;
}

bool TrackContainer::add_det2track(size_t track_id, const Detection& det) {
    assert (det.frame < video_len_);
    assert (video_len_ == timeline_.size());

    for (auto track_iter = tracks_.begin(); track_iter != tracks_.end(); ++track_iter) {
        if (track_iter->get_id() == track_id) {
            Detection* det_ptr = track_iter->add(det);
            timeline_[det.frame].push_back(det_ptr);
            return true;
        }
    }
    return false;
}

std::vector<Detection> TrackContainer::get_detections(size_t frame_idx) const {
    if (frame_idx >= video_len_) {
        return std::vector<Detection>();
    }

    std::vector<Detection> detections;
    for (const auto& detection_ptr : timeline_[frame_idx]) {
        detections.push_back(*detection_ptr);
    }
    return detections;
}

std::vector<DetectionAndTrack> TrackContainer::get_tracks_and_detections(size_t frame_idx,
                                                         size_t max_frames_before,
                                                         size_t max_frames_after) {
    if (frame_idx >= video_len_) {
        return std::vector<DetectionAndTrack>();
    }

    std::vector<Detection> frame_detections = this->get_detections(frame_idx);

    std::vector<DetectionAndTrack> det_and_tracks;

    for (auto det : frame_detections) {
        auto track = this->get_track(det.id);
        assert(track != nullptr);
        det_and_tracks.push_back(track->make_detection_track(frame_idx, max_frames_before, max_frames_after));
    }

    return det_and_tracks;
}

std::map<size_t, std::vector<Detection>> TrackContainer::get_slice(size_t min_frame_idx, size_t max_frame_idx) const {
    std::map<size_t, std::vector<Detection>> tracks;

    assert(min_frame_idx <= max_frame_idx);
    assert(max_frame_idx < this->video_len_);

    for (size_t frame_idx = min_frame_idx; frame_idx < max_frame_idx; ++frame_idx) {
        for (const auto& det : this->get_detections(frame_idx)) {
            tracks[det.id].push_back(det);
        }
    }

    return tracks;
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

bool TrackContainer::load(const std::string& filepath) {
    char comma;

    std::ifstream stream_(filepath);
    if (!stream_.is_open()) {
        std::cerr << "File not found:" << filepath;
        return false;
    }

    std::string line;
    // size_t max_frame = 0;
    bool is_initalized = false;

    size_t line_idx = 0;
    while (std::getline(stream_, line))
    {
        std::istringstream iss(line);
        Detection det;
        size_t object_type;

        if (line_idx == 0) {
            iss >> this->video_len_;
            line_idx++;
            timeline_.resize(this->video_len_);
            continue;
        }

        if (!(iss
                >> det.frame >> comma
                >> det.id >> comma
                >> det.bbox.x >> comma
                >> det.bbox.y >> comma
                >> det.bbox.width >> comma
                >> det.bbox.height >> comma
                >> det.confidence >> comma
                >> object_type)) { break; } // error

        line_idx++;

        // max_frame = std::max(max_frame, det.frame);

        if (object_type != pedestrian_class_) {
            continue;
        }

        det.frame -= 1;  // Frame indices starts from 1

        if (!this->has_track(det.id)) {
            Track new_track(det.id);
            this->add_track(new_track);
        }

        assert(this->add_det2track(det.id, det));

        is_initalized = true;
    }

    assert(is_initalized);

    // this->video_len_ = max_frame + 1;

    return true;
}

bool TrackContainer::save(const std::string& filepath) {
    char comma = ',';

    std::ofstream stream_(filepath);
    if (!stream_.is_open()) {
        std::cerr << "File not found:" << filepath;
        return false;
    }

    stream_ << video_len_ << std::endl;

    for (size_t frame_index = 0; frame_index < this->get_video_len(); ++frame_index) {
        std::vector<Detection> detections = get_detections(frame_index);

        for (const auto& det : detections) {
            stream_ << det.frame + 1 << comma
                    << det.id << comma
                    << det.bbox.x << comma
                    << det.bbox.y << comma
                    << det.bbox.width << comma
                    << det.bbox.height << comma
                    << det.confidence << comma
                    << pedestrian_class_ << std::endl;
        }
    }
    return true;
}

bool TrackContainer::unite_tracks(size_t id_A, size_t id_B) {
    std::unique_ptr<Track> track_A = this->get_track(id_A);
    if (track_A == nullptr) {
        return false;
    }

    std::unique_ptr<Track> track_B = this->get_track(id_B);
    if (track_B == nullptr) {
        return false;
    }

    size_t new_id = std::min(track_A->get_id(), track_B->get_id());
    size_t delete_id = std::max(track_A->get_id(), track_B->get_id());
    Track united_tracks(new_id);

    for (const auto& det : *track_A) {
        auto det_cpy = det;
        det_cpy.id = new_id;
        united_tracks.push_back(det_cpy);
    }

    for (const auto& det : *track_B) {
        auto det_cpy = det;
        det_cpy.id = new_id;
        united_tracks.add(det_cpy);
    }

    this->delete_track(new_id);
    this->delete_track(delete_id);

    this->add_track(united_tracks);

    return true;
}

bool TrackContainer::delete_detection(size_t track_id, size_t frame_idx) {
    if (!this->has_track(track_id)) {
        return false;
    }

    if (frame_idx >= this->video_len_) {
        return false;
    }

    assert(timeline_.size() == this->video_len_);

    for (auto det_ptr_it = timeline_[frame_idx].begin();
         det_ptr_it != timeline_[frame_idx].end();
         ++det_ptr_it) {
        if ((*det_ptr_it)->id == track_id) {
            timeline_[frame_idx].erase(det_ptr_it);

           break;
        }
    }

    for (auto track_it = tracks_.begin(); track_it != tracks_.end(); ++track_it) {
        if (track_it->get_id() == track_id) {
            track_it->del(frame_idx);
        }
    }



    return true;
}
