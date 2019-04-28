#define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include "opencv2/core/core.hpp"

#include "markup_backend/utils.h"
#include "markup_backend/markup.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(markup_suit)

BOOST_AUTO_TEST_CASE(markup_logic)
{
    std::string video_path = "../../../markup_tool/data/test/MOT16-04/img1/";
    std::string model_path = "../../../markup_tool/python/detector/yolo3.py";
    std::string weights_path = "../../../markup_tool/data/models/yolo3/";
    size_t video_len = 5;

    std::unique_ptr<MarkUp> mark_up;
    BOOST_CHECK_NO_THROW(mark_up = std::make_unique<MarkUp>());

    BOOST_CHECK_NO_THROW(mark_up->set_model(model_path, weights_path));

    // Check defaults
    PipelineRunParams params;
    BOOST_CHECK_NO_THROW(params = mark_up->get_params());
    BOOST_CHECK(exists(params.tracker_model_path));

    std::vector<Detection> detections;
    // track_container_ == nullptr
    BOOST_CHECK(!mark_up->get_frame(0, &detections));
    BOOST_CHECK(detections.empty());

    BOOST_CHECK_EQUAL(mark_up->get_video_len(), 0);

    BOOST_CHECK(!mark_up->run());

    BOOST_CHECK(!mark_up->set_video(""));
    BOOST_CHECK(!mark_up->set_video("/tmp/deadbeaf"));
    BOOST_CHECK(mark_up->set_video(video_path));
    BOOST_CHECK_EQUAL(mark_up->get_video_len(), 5);

    // frame_idx >= track_container_->get_video_len()
    size_t not_real_index = 100000;
    BOOST_CHECK_GT(not_real_index, video_len);
    BOOST_CHECK(!mark_up->get_frame(not_real_index, &detections));
    BOOST_CHECK(detections.empty());

    /*BOOST_CHECK(mark_up->run());
    for (size_t frame_idx = 0; frame_idx < mark_up->get_video_len(); ++frame_idx) {
        BOOST_CHECK(mark_up->get_frame(frame_idx, &detections));
        BOOST_CHECK(!detections.empty());
        for (const auto& det : detections) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
        }
    }*/
}

BOOST_AUTO_TEST_SUITE_END()

