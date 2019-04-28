#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include "boost/filesystem.hpp"

#include "markup_backend/nn_model.h"
#include "markup_backend/markup.h"
#include "markup_backend/utils.h"

std::unique_ptr<TrackContainer> MarkUp::run_pipeline(const Video& video) {
    if (boost::filesystem::exists(params_.tmp_video_dir)) {
        boost::filesystem::remove_all(params_.tmp_video_dir);
    }

    boost::filesystem::create_directory(params_.tmp_video_dir);
    assert(boost::filesystem::exists(params_.tmp_video_dir));

    // TODO: Rewrite with Video iterators;
    for (size_t frame_index = 0; frame_index < video.size(); ++frame_index) {
        cv::Mat img = video.get_frame(frame_index);

        std::string filename = std::to_string(frame_index);

        // TODO: raise proper error
        assert(filename.length() <= params_.tmp_img_path_pad2length);

        filename = std::string(params_.tmp_img_path_pad2length - filename.length(), '0')
                   + filename
                   + params_.tmp_img_extention;

        cv::imwrite((boost::filesystem::path(params_.tmp_video_dir) / boost::filesystem::path(filename)).string(), img);
    }

    std::string tracks_filepath
        = (boost::filesystem::path(params_.tmp_video_dir) / boost::filesystem::path(params_.tracks_filename)).string();

    if (!boost::filesystem::exists(params_.tracker_model_path)) {
        std::cout << "No such tracker model:" << params_.tracker_model_path << std::endl;
        return nullptr;
    }

    if (!boost::filesystem::exists(params_.weights_path)) {
        std::cout << "No such weights dir:" << params_.weights_path << std::endl;
        return nullptr;
    }

    std::string delimiter = " ";
    std::string command = "python3" + delimiter;
    command += params_.tracker_model_path + delimiter;
    command += "--config_dir" + delimiter + params_.weights_path + delimiter;
    command += "--imgs_dir" + delimiter + params_.tmp_video_dir + delimiter;
    command += "--tracks_filepath" + delimiter + tracks_filepath + delimiter;

    system(command.c_str());

    assert(boost::filesystem::exists(tracks_filepath));

    // std::unique_ptr<TrackContainer> track_container(new TrackContainer(tracks_filepath));

    return std::make_unique<TrackContainer>(tracks_filepath);
}

bool MarkUp::get_frame(size_t frame_idx, std::vector<Detection>* detections) {
    if (detections == nullptr) {
        // TODO: Exceptions
        std::cout << "No return container 'detections' provided!" << std::endl;
        return false;
    }

    detections->clear();

    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (frame_idx >= track_container_->get_video_len()) {
        // TODO: Exceptions
        std::cout << "No such frame:" << frame_idx
                  << " Only has " << track_container_->get_video_len() << std::endl;
        return false;
    }

    *detections = track_container_->get_detections(frame_idx);

    return true;
}

bool MarkUp::set_model(const std::string& model_path,
               const std::string& weights_path) {
    if (model_path.empty() || weights_path.empty()) {
        return false;
    }

    if (!boost::filesystem::exists(model_path)) {
        // TODO: Exceptions
        std::cout << "File not found:" << model_path << std::endl;
        return false;
    }

    if (!boost::filesystem::exists(weights_path)) {
        // TODO: Exceptions
        std::cout << "File not found:" << weights_path << std::endl;
        return false;
    }

    params_.tracker_model_path = model_path;
    params_.weights_path = weights_path;
}

bool MarkUp::set_video(const std::string& filepath) {
    if (filepath.empty()) {
        return false;
    }

    if (!boost::filesystem::exists(filepath)) {
        // TODO: Exceptions
        std::cout << "File not found:" << filepath << std::endl;
        return false;
    }

    if (params_.video_path == filepath) {
        return true;
    }

    // Clear state
    video_.reset(nullptr);
    track_container_.reset(nullptr);

    params_.video_path = filepath;
    video_ = std::make_unique<Video>(params_.video_path);
}

bool MarkUp::run() {
    track_container_.reset(nullptr);

    if (!boost::filesystem::exists(params_.tracker_model_path)) {
        // TODO: Exceptions
        std::cout << "Model doesnt exist:" << params_.tracker_model_path << std::endl;
        return false;
    }

    if (video_ == nullptr) {
        // TODO: Exceptions
        std::cout << "Set Video first!" << std::endl;
        return false;
    }

    // track_container_ = this->run_pipeline(*video_);

    /* if (boost::filesystem::exists(params_.debug_gt_tacks)) {
        // TODO: Exceptions
        std::cout << "No debug tracks:" << params_.debug_gt_tacks << std::endl;
        return false;
    }*/

    track_container_ = read_dummy_trackcontainer(params_.debug_gt_tacks);

    return true;
}

bool MarkUp::unite_tracks(size_t track_id_one, size_t track_id_two) {
    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (!this->track_container_->has_track(track_id_one)) {
        // TODO: Exceptions
        std::cout << "No such track found:" << track_id_one << std::endl;
        return false;
    }

    if (!this->track_container_->has_track(track_id_two)) {
        // TODO: Exceptions
        std::cout << "No such track found:" << track_id_two<< std::endl;
        return false;
    }

    return track_container_->unite_tracks(track_id_one, track_id_two);
}

bool MarkUp::delete_track(size_t id) {
    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (!this->track_container_->has_track(id)) {
        // TODO: Exceptions
        std::cout << "No such track found:" << id << std::endl;
        return false;
    }

    return this->track_container_->delete_track(id);
}

bool MarkUp::add_detection(const Detection& det) {
    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (this->track_container_->has_track(det.id)) {
        // TODO: Exceptions
        std::cout << "Already has this track:" << det.id << std::endl;
        return false;
    }

    if (det.frame >= track_container_->get_video_len()) {
        // TODO: Exceptions
        std::cout << "No such frame:" << det.frame
                  << " Only has " << track_container_->get_video_len() << std::endl;
        return false;
    }

    return this->track_container_->add_det2track(det.id, det);
}

size_t MarkUp::get_video_len() const {
    if (video_ == nullptr) {
        return 0;
    } else {
        return video_->size();
    }
}

PipelineRunParams MarkUp::get_params() const {
    return params_;
}

bool MarkUp::delete_detection(size_t track_id, size_t frame_idx) {
    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (!this->track_container_->has_track(track_id)) {
        // TODO: Exceptions
        std::cout << "Track not found:" << track_id << std::endl;
        return false;
    }

    if (frame_idx >= track_container_->get_video_len()) {
        // TODO: Exceptions
        std::cout << "No such frame:" << frame_idx
                  << " Only has " << track_container_->get_video_len() << std::endl;
        return false;
    }

    return track_container_->delete_detection(track_id, frame_idx);
}

bool MarkUp::load_markup(const std::string& path2file) {
    track_container_.reset(nullptr);

    if (!boost::filesystem::exists(path2file)) {
        // TODO: Exceptions
        std::cout << "No such file found:" << path2file << std::endl;
        return false;
    }

    track_container_ = std::make_unique<TrackContainer>(path2file);
    return true;
}

bool MarkUp::save_markup(const std::string& path2file) {
    if (track_container_ == nullptr) {
        // TODO: Exceptions
        std::cout << "First compute tracks!" << std::endl;
        return false;
    }

    if (!boost::filesystem::exists(path2file)) {
        // TODO: Exceptions
        std::cout << "No such file found:" << path2file << std::endl;
        return false;
    }

    return track_container_->save(path2file);
}
