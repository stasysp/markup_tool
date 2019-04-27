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

BOOST_AUTO_TEST_CASE(io_track_container)
{
    size_t video_len = 3;
    size_t num_tracks = 2;
    std::string dump_init_filename = "/tmp/dump_init.txt";

    std::unique_ptr<TrackContainer> track_container;
    {
        std::unique_ptr<Track> track_1;
        BOOST_CHECK_NO_THROW(track_1 = std::make_unique<Track>(1));

        {
            size_t track_id = 1;

            BOOST_CHECK_NO_THROW(track_1 = std::make_unique<Track>(track_id));

            for (size_t i = 0; i < video_len; ++i) {
                Detection det;
                det.id = track_id;
                det.frame = i;
                BOOST_CHECK_NO_THROW(track_1->push_back(det));
            }
        }

        std::unique_ptr<Track> track_2;


        {
            size_t track_id = 2;

            BOOST_CHECK_NO_THROW(track_2 = std::make_unique<Track>(track_id));
            for (size_t i = 0; i < video_len; ++i) {
                Detection det;
                det.id = track_id;
                det.frame = i;
                BOOST_CHECK_NO_THROW(track_2->push_back(det));
            }
        }


        BOOST_CHECK_NO_THROW(track_container = std::make_unique<TrackContainer>(video_len));
        BOOST_CHECK_NO_THROW(track_container->add_track(*track_1));
        BOOST_CHECK_NO_THROW(track_container->add_track(*track_2));
        BOOST_CHECK_EQUAL(track_container->get_video_len(), video_len);
    }

    {
        for (size_t frame_idx = 0;
                 frame_idx < track_container->get_video_len();
                 ++frame_idx) {
            std::vector<Detection> detections;
            BOOST_CHECK_NO_THROW(detections = track_container->get_detections(frame_idx));
            BOOST_CHECK_EQUAL(detections.size(), num_tracks);
            for (const auto& det : detections) {
                BOOST_CHECK_EQUAL(det.frame, frame_idx);
                BOOST_CHECK(1 <= det.id && det.id <= num_tracks);
            }
        }
    }

    BOOST_CHECK_NO_THROW(track_container->save(dump_init_filename));
    BOOST_CHECK(boost::filesystem::exists(dump_init_filename));

    std::unique_ptr<TrackContainer> control_container;
    BOOST_CHECK_NO_THROW(control_container = std::make_unique<TrackContainer>(dump_init_filename));
    BOOST_CHECK(!control_container->empty());
    BOOST_CHECK_EQUAL(control_container->get_video_len(), video_len);
    BOOST_CHECK_EQUAL(control_container->get_num_tracks(), num_tracks);

    for (size_t frame_idx = 0; frame_idx < video_len; ++frame_idx) {
        std::vector<Detection> detections;
        BOOST_CHECK_NO_THROW(detections = control_container->get_detections(frame_idx));
        BOOST_CHECK_EQUAL(detections.size(), num_tracks);
        for (const auto& det : detections) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            BOOST_CHECK(1 <= det.id && det.id <= num_tracks);
        }
    }


    /*BOOST_CHECK_NO_THROW(track_container->save(dump_init_filename));

    BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(1));
    BOOST_CHECK(det_ptr == nullptr);

    for (size_t i = 0; i < 10; ++i) {
        BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(0));
        BOOST_CHECK(det_ptr != nullptr);

        BOOST_CHECK_EQUAL(det_ptr->frame, 0);
        BOOST_CHECK_EQUAL(det_ptr->id, det.id);
    }*/
}

BOOST_AUTO_TEST_SUITE_END()

