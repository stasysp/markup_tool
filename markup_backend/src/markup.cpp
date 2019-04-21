#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include "boost/filesystem.hpp"

#include "markup_backend/nn_model.h"
#include "markup_backend/markup.h"

TrackContainer MarkUp::run(const Video& video) {
    if (boost::filesystem::exists(params_.tmp_video_dir)) {
        boost::filesystem::remove_all(params_.tmp_video_dir);
    }

    boost::filesystem::create_directory(params_.tmp_video_dir);

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

    std::string delimiter = " ";
    std::string command = "python" + delimiter;
    command += params_.tracker_model_path + delimiter;
    command += params_.tmp_video_dir + delimiter;
    command += params_.tmp_video_dir + delimiter;
    system(command.c_str());


}

