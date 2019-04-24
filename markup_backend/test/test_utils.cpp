// #define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "markup_backend/utils.h"
#include "markup_backend/video.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(utils_suit)

BOOST_AUTO_TEST_CASE(working_with_files)
{
    std::string dirpath = "/tmp/delete_me";

    for (size_t i = 0; i < 2; ++i) {
        BOOST_CHECK_NO_THROW(make_clear_dir(dirpath));

        std::ofstream out_stream(dirpath + "/" + "/test.txt");
        BOOST_CHECK(out_stream.is_open());
        out_stream << 42;
        out_stream.close();

        std::ifstream in_stream(dirpath + "/" + "test.txt");
        BOOST_CHECK(in_stream.is_open());
        size_t read_value;
        in_stream >> read_value;
        BOOST_CHECK_EQUAL(read_value, 42);
        in_stream.close();
    }
}

BOOST_AUTO_TEST_CASE(read_det_from_file)
{

    std::string det_filepath = "../../../markup_tool/data/test/MOT16-04/gt/gt.txt";
    std::string video_filepath = "../../../markup_tool/data/test/MOT16-04/img1";
    std::string debug_detections_dir = "/Users/user/Desktop/detections";
    cv::Size test_img_size(1920, 1080);

    std::unique_ptr<Video> video;
    BOOST_CHECK_NO_THROW(video = std::make_unique<Video>(video_filepath));
    BOOST_CHECK(video != nullptr);
    BOOST_CHECK_GT(video->size(), 0);


    std::unique_ptr<TrackContainer> detected_tracks;

    BOOST_CHECK_NO_THROW(detected_tracks = read_dummy_trackcontainer(det_filepath));

    BOOST_CHECK_EQUAL(detected_tracks->get_video_len(), video->size());

    BOOST_CHECK(!detected_tracks->empty());

    // BOOST_CHECK_EQUAL(detected_tracks->get_track(10000000), nullptr);
    BOOST_CHECK(detected_tracks->get_track(10000000) == nullptr);

    std::unique_ptr<Track> track;
    size_t test_track_index = 1;
    BOOST_CHECK_NO_THROW(track = detected_tracks->get_track(test_track_index));
    BOOST_CHECK(track != nullptr);

    size_t frame_idx = 0;
    for (const auto& detection : *track) {
        BOOST_CHECK_EQUAL(detection.id, test_track_index);
        BOOST_CHECK_EQUAL(detection.frame, frame_idx);
        frame_idx++;
    }

    std::vector<Detection> frame_detections;
    // size_t frame_index = 0;
    // BOOST_CHECK_NO_THROW(frame_detections = detected_tracks->get_detections(frame_index));
    // BOOST_CHECK(!frame_detections.empty());
    // for (const auto& detection : frame_detections) {
    //     BOOST_CHECK_EQUAL(detection.frame, frame_index);
    // }

    BOOST_CHECK_EQUAL(detected_tracks->get_video_len(), video->size());

    std::vector<std::string> found_filenames;
    BOOST_CHECK_NO_THROW(found_filenames = video->get_frame_names());
    BOOST_CHECK_EQUAL(detected_tracks->get_video_len(), found_filenames.size());

    BOOST_CHECK_NO_THROW(make_clear_dir(debug_detections_dir));

    for (size_t frame_idx = 0; frame_idx < video->size(); ++frame_idx) {
        cv::Mat frame;
        BOOST_CHECK_NO_THROW(frame = video->get_frame(frame_idx));
        BOOST_CHECK(frame.size() == test_img_size);
        BOOST_CHECK(frame.type() == CV_8UC3);

        BOOST_CHECK_NO_THROW(frame_detections = detected_tracks->get_detections(frame_idx));
        BOOST_CHECK(!frame_detections.empty());

        for (const auto& detection : frame_detections) {
            BOOST_CHECK_EQUAL(detection.frame, frame_idx);

            /// It's ok when bounding box
//            BOOST_CHECK_GE(detection.bbox.x, 0);
//            BOOST_CHECK_GE(detection.bbox.y, 0);
//
//            BOOST_CHECK_LT(detection.bbox.x, test_img_size.width);
//            BOOST_CHECK_LT(detection.bbox.y, test_img_size.height);
//
//            BOOST_CHECK_GE(detection.bbox.width, 0);
//            BOOST_CHECK_GE(detection.bbox.height, 0);
//
//            BOOST_CHECK_LT(detection.bbox.x + detection.bbox.width, test_img_size.width);
//            BOOST_CHECK_LT(detection.bbox.y + detection.bbox.height, test_img_size.height);

            cv::rectangle(frame, detection.bbox, cv::Scalar(0, 0, 255));
        }

        cv::imwrite((path(debug_detections_dir) / path(found_filenames[frame_idx])).string(),
                frame);
    }

}

BOOST_AUTO_TEST_SUITE_END()
