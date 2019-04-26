#include <fstream>

#include "markup_backend/tracks.h"



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

std::unique_ptr<Track> TrackContainer::get_track(size_t id) {
    for (auto track_iter = tracks_.begin(); track_iter != tracks_.end(); ++track_iter) {
        if (track_iter->get_id() == id) {
            return std::make_unique<Track>(*track_iter);
        }
    }

    return nullptr;
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
    while (std::getline(stream_, line))
    {
        // if (line.empty()) break;

        std::istringstream iss(line);
        Detection det;
        size_t object_type;

        if (!(iss
                >> det.frame >> comma
                >> det.id >> comma
                >> det.bbox.x >> comma
                >> det.bbox.y >> comma
                >> det.bbox.width >> comma
                >> det.bbox.height >> comma
                >> det.confidence >> comma
                >> object_type)) { break; } // error

        if (object_type != pedestrian_class_) {
            continue;
        }

        det.frame -= 1;  // Frame indices starts from 1

        std::unique_ptr<Track> track = this->get_track(det.id);

        if (track == nullptr) {
            Track new_track(det.id);
            this->add_track(new_track);
            track = this->get_track(det.id);
            assert(track != nullptr);
        }

        std::unique_ptr<Detection> last_detection = track->get_last_detection();
        if (last_detection != nullptr) {
            assert(last_detection->frame < det.frame);
        }

        track->push_back(det);
    }

    return true;
}

bool TrackContainer::save(const std::string& filepath) {
    char comma = ',';

    std::ofstream stream_(filepath);
    if (!stream_.is_open()) {
        std::cerr << "File not found:" << filepath;
        return false;
    }

    for (size_t frame_index = 0; frame_index < this->get_video_len(); ++frame_index) {
        std::vector<Detection> detections = get_detections(frame_index);

        for (const auto& det : detections) {
            stream_ << det.frame << comma
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
