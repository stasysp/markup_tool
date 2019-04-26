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

    std::cout << command << std::endl;

    system(command.c_str());

    assert(boost::filesystem::exists(tracks_filepath));

    std::cout << "test:" << command << std::endl;

    //

    std::unique_ptr<TrackContainer> track_container(new TrackContainer(tracks_filepath));
}

