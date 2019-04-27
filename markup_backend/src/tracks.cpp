#include <fstream>

#include "markup_backend/tracks.h"



// ---------------- class Track --------------------------

Detection* Track::push_back(const Detection& det) {
    assert(det.id == id_);

    detections_.push_back(det);

    return &detections_.back();
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

// ---------------- class TrackContainer -----------------

TrackContainer::TrackContainer(size_t video_length) {
    this->video_len_ = video_length;
    timeline_.resize(this->video_len_);
}

TrackContainer::TrackContainer(const std::string& tracks_filepath) {
    assert(this->load(tracks_filepath));
}

void TrackContainer::add_track(const Track& track) {
    // Checks
    tracks_.push_back(track);
    for (std::list<Detection>::iterator it = tracks_.back().begin(); it != tracks_.back().end(); ++it) {
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
}
