// #define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include "opencv2/core/core.hpp"

#include "markup_backend/utils.h"
#include "markup_backend/markup.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(tracker_suit)

BOOST_AUTO_TEST_CASE(markup)
{
    /*PipelineRunParams params;
    params.tracker_model_path = "../../../markup_tool/python/detector/yolo3.py";
    std::string test_video_dir = "../../../markup_tool/data/test/MOT16-04/img1/";

    std::unique_ptr<MarkUp> mark_up;
    BOOST_CHECK_NO_THROW(mark_up = std::make_unique<MarkUp>(params));

    std::unique_ptr<Video> video;
    BOOST_CHECK_NO_THROW(video = std::make_unique<Video>(test_video_dir));

    std::unique_ptr<TrackContainer> track_container;
    // BOOST_CHECK_NO_THROW(track_container = mark_up->run(*video));*/
}

BOOST_AUTO_TEST_CASE(basic_track_ops)
{
    size_t track_index = 42;
    std::unique_ptr<Track> track;

    BOOST_CHECK_NO_THROW(track = std::make_unique<Track>(track_index));

    {
        Detection det;
        det.id = track_index;

        det.frame = 0;
        BOOST_CHECK_NO_THROW(track->push_back(det));

        det.frame = 3;
        BOOST_CHECK_NO_THROW(track->push_back(det));

        det.frame = 1;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 2;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 4;
        BOOST_CHECK_NO_THROW(track->add(det));

        BOOST_CHECK_EQUAL(track->get_last_detection()->frame, 4);
    }

    size_t frame_idx = 0;
    for (const auto& det : *track) {
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx++;
    }


    BOOST_CHECK_EQUAL(track->get_id(), track_index);

    {
        Detection det;
        det.id = track_index;

        BOOST_CHECK_NO_THROW(track->del(0));
        BOOST_CHECK_NO_THROW(track->del(2));
        BOOST_CHECK_NO_THROW(track->del(10000));
        BOOST_CHECK_NO_THROW(track->del(4));
    }

    frame_idx = 1;
    for (const auto& det : *track) {
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx += 2;
    }

    {
        Detection det;
        det.id = track_index;

        det.frame = 0;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 1;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 2;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 4;
        BOOST_CHECK_NO_THROW(track->add(det));

        BOOST_CHECK_EQUAL(track->get_last_detection()->frame, 4);
    }

    frame_idx = 0;
    for (const auto& det : *track) {
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx++;
    }

    {
        Detection det;
        det.id = track_index;

        det.frame = 5;
        BOOST_CHECK_NO_THROW(track->push_back(det));

        det.frame = 6;
        BOOST_CHECK_NO_THROW(track->push_back(det));

        det.frame = 7;
        BOOST_CHECK_NO_THROW(track->add(det));

        det.frame = 8;
        BOOST_CHECK_NO_THROW(track->push_back(det));

        BOOST_CHECK_EQUAL(track->get_last_detection()->frame, 8);
    }

    frame_idx = 0;
    for (const auto& det : *track) {
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx++;
    }

    for (int i = 10; i >= 0; --i) {
        BOOST_CHECK_NO_THROW(track->del(i));
    }

    frame_idx = 0;
    for (const auto& det : *track) {
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx += 2;
    }
}

BOOST_AUTO_TEST_CASE(tracks)
{
    size_t track_index = 42;

    std::unique_ptr<Track> track;
    BOOST_CHECK_NO_THROW(track = std::make_unique<Track>(track_index));

    Detection det;
    det.frame = 0;
    det.id = track_index;

    BOOST_CHECK_NO_THROW(track->push_back(det));
    std::unique_ptr<Detection> det_ptr;
    BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(1));
    BOOST_CHECK(det_ptr == nullptr);

    for (size_t i = 0; i < 10; ++i) {
        BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(0));
        BOOST_CHECK(det_ptr != nullptr);

        BOOST_CHECK_EQUAL(det_ptr->frame, 0);
        BOOST_CHECK_EQUAL(det_ptr->id, det.id);
    }

    size_t max_frame_idx = 4;
    for (size_t i = 1; i < max_frame_idx; ++i) {
        Detection det;
        det.frame = i;
        det.id = track_index;

        BOOST_CHECK_NO_THROW(track->push_back(det));
    }

    size_t frame_idx = 0;
    for (std::list<Detection>::const_iterator it = track->begin();
            it != track->end(); ++it) {
        Detection det = *it;
        BOOST_CHECK_EQUAL(track_index, det.id);
        BOOST_CHECK_EQUAL(det.frame, frame_idx);
        frame_idx++;
    }

    for (frame_idx = 0; frame_idx < max_frame_idx; ++frame_idx) {
        BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(frame_idx));
        BOOST_CHECK_EQUAL(track_index, det_ptr->id);
        BOOST_CHECK_EQUAL(det_ptr->frame, frame_idx);
    }

    BOOST_CHECK_NO_THROW(det_ptr = track->get_last_detection());
    BOOST_CHECK_EQUAL(track_index, det_ptr->id);
    BOOST_CHECK_EQUAL(det_ptr->frame, max_frame_idx - 1);
}



BOOST_AUTO_TEST_SUITE_END()

