#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "markup_backend/definitions.h"
#include "markup_backend/tracks.h"


// "Read from det.txt from MOT16 data
std::vector<std::vector<Detection>> read_mot16_detections(const std::string& det_filepath);

// Read from det.txt from MOT16 data
std::unique_ptr<TrackContainer> read_dummy_trackcontainer(const std::string& tracks_filepath);

void make_clear_dir(const std::string& dirpath);