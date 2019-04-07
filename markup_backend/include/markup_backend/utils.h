#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "markup_backend/definitions.h"


// "Read from det.txt from MOT16 data
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

// "Read from det.txt from MOT16 data
std::vector<std::vector<Detection>> read_mot16_tracks(const std::string& tracks_filepath) {
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    char comma;

    std::ifstream tracks_stream(tracks_filepath);
    if (!tracks_stream.is_open()) {
        std::cerr << "File not found:" << tracks_filepath;
        return std::vector<std::vector<Detection>>();
    }

    std::vector<std::vector<Detection>> read_detections;

    std::string line;
    std::unique_ptr<Track> track = nullptr;
    while (std::getline(det_file, line))
    {
        std::istringstream iss(line);
        Detection det;
        size_t object_type;
        if (!(det_file
                >> det.frame >> comma
                >> det.id >> comma
                >> det.bbox.x >> comma
                >> det.bbox.y >> comma
                >> det.bbox.width >> comma
                >> det.bbox.height >> comma
                >> det.confidence >> comma
                >> object_type)) { break; } // error

        if ()


        det.frame -= 1;  // Frame indices starts from 1

        if (det.frame == 0) {
            track = std::make_unique<Track>(det.id);
        }

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