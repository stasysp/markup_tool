#include <boost/filesystem.hpp>

#include "markup_backend/utils.h"

std::vector<std::vector<Detection>> read_mot16_detections(const std::string& det_filepath) {
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    char comma;

    std::ifstream det_file(det_filepath);
    if (!det_file.is_open()) {
        std::cerr << "File not found:" << det_filepath;
        return std::vector<std::vector<Detection>>();
    }

    std::vector<std::vector<Detection>> read_detections;

    std::string line;
    while (std::getline(det_file, line))
    {
        std::istringstream iss(line);
        Detection det;
        if (!(det_file
                >> det.frame >> comma
                >> det.id >> comma
                >> det.bbox.x >> comma
                >> det.bbox.y >> comma
                >> det.bbox.width >> comma
                >> det.bbox.height >> comma
                >> det.confidence)) { break; } // error


        det.frame -= 1;  // Frame indices starts from 1

        // process pair (a,b)
        if (det.frame == read_detections.size()) {
            read_detections.emplace_back(std::vector<Detection>());
        }

        if (det.frame < read_detections.size()) {
            read_detections[det.frame].push_back(det);
        } else {
            std::cerr << "File corrupted:" << det_filepath;
            return std::vector<std::vector<Detection>>();
        }
    }

    return read_detections;
};

std::unique_ptr<TrackContainer> read_dummy_trackcontainer(const std::string& tracks_filepath) {
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c

    char comma;
    size_t pedestrian = 1;
    size_t min_det_frame = 0;
    size_t max_det_frame = 5;

    // TrackContainer aligned_tracks(max_det_frame);
    std::unique_ptr<TrackContainer> read_tracks(new TrackContainer(max_det_frame));

    std::ifstream tracks_stream(tracks_filepath);
    if (!tracks_stream.is_open()) {
        std::cerr << "File not found:" << tracks_filepath;
        return nullptr;
    }

    std::string line;
    std::unique_ptr<Track> track = nullptr;
    while (std::getline(tracks_stream, line))
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

        if (object_type != pedestrian) {
            continue;
        }


        det.frame -= 1;  // Frame indices starts from 1

        if (det.frame == 0) {
            if (track != nullptr) {
                read_tracks->add_track(*track);
            }
            track = std::make_unique<Track>(det.id);
        }

        if (det.frame < max_det_frame) {
            track->push_back(det);
        } else {
            continue;
        }
    }

    return read_tracks;
};

void make_clear_dir(const std::string& dirpath) {
    if (boost::filesystem::exists(dirpath)) {
        boost::filesystem::remove_all(dirpath);
    }

    boost::filesystem::create_directory(dirpath);
}
