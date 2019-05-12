#pragma once

#include <string>
#include <tuple>

#include "markup_backend/definitions.h"
#include "markup_backend/tracks.h"
#include "markup_backend/video.h"
#include "markup_backend/utils.h"

// #include <boost/filesystem.hpp>
// using namespace boost::filesystem;

struct PipelineRunParams {
    // Путь до видео которое процессится задается через интерфейс;
    std::string video_path = "data/test/MOT16-04/img1";
    // Примеры путей
    std::string tracker_model_path = "python/detector/yolo3.py";
    // Примеры путей
    std::string weights_path = "data/models/yolo3";
    std::string tmp_video_dir = "/tmp/markup_video";
    std::string tracks_path = "data/test/MOT16-04/gt/gt.txt";
    std::string tmp_img_extention = ".png";
    size_t tmp_img_path_pad2length = 10;
    std::string tracks_filename = "tracks.txt";
};

class MarkUp {
public:
    bool get_frame(size_t frame_idx,
                   std::vector<Detection>* detections) const;

    bool get_frame(size_t frame_idx,
                   std::vector<DetectionAndTrack>* detections,
                   size_t max_frames_before=20, size_t max_frames_after=20) const;

    // Returns [a, b)
    bool get_slice(size_t min_frame_idx, size_t max_frame_idx,
                    std::map<size_t, std::vector<Detection>>* tracks) const;

    bool set_model(const std::string& model_path,
                   const std::string& weights_path);

    bool set_video(const std::string& filepath);

    bool set_tracks(const std::string& filepath);

    bool run();

    bool split_track(size_t track_id, size_t frame2split_idx);

    bool interpolate_track(size_t track_id, size_t from_frame_idx, size_t to_frame_idx);

    bool unite_tracks(size_t track_id_one, size_t track_id_two);

    bool delete_track(size_t id);

    size_t add_detection(const Detection& det);
    size_t add_detection(size_t frame, int xmin, int ymin, int xmax, int ymax);

    bool delete_detection(size_t track_id, size_t frame_idx);

    size_t get_video_len() const;
    std::tuple<size_t, size_t> get_frame_shape() const;

    PipelineRunParams get_params() const;

    bool load_markup(const std::string& path2file);

    bool save_markup(const std::string& path2file);



private:
    PipelineRunParams params_;
    std::unique_ptr<Video> video_ = nullptr;
    std::unique_ptr<TrackContainer> track_container_ = nullptr;

    std::unique_ptr<TrackContainer> run_pipeline(const Video& video);
};


/*class MarkUp {
public:
    MarkUp() = delete;
    explicit MarkUp(const PipelineRunParams& params) : params_(params) {
    }

    std::unique_ptr<TrackContainer> run(const Video& video);
    
private:

    PipelineRunParams params_;

    // Detector detector_;
    // IDModel id_model_;
};*/
