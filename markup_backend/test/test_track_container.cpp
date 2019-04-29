// #define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include "opencv2/core/core.hpp"

#include "markup_backend/utils.h"
#include "markup_backend/markup.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(track_container)

BOOST_AUTO_TEST_CASE(split_track)
{
    size_t video_len = 5;
    std::unique_ptr<TrackContainer> track_container;
    BOOST_CHECK_NO_THROW(track_container = std::make_unique<TrackContainer>(video_len));

    size_t track_id = 42;
    {
        Track track(track_id);
        Detection det;
        det.id = track_id;

        det.frame = 1;
        BOOST_CHECK_NO_THROW(track.push_back(det));

        det.frame = 2;
        BOOST_CHECK_NO_THROW(track.push_back(det));

        det.frame = 3;
        BOOST_CHECK_NO_THROW(track.push_back(det));

        BOOST_CHECK_NO_THROW(track_container->add_track(track));
    }

    BOOST_CHECK(track_container->has_track(track_id));
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 1);

    bool ret;
    BOOST_CHECK_NO_THROW(ret = track_container->split_track(track_id, 0));
    BOOST_CHECK(!ret);
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 1);
    BOOST_CHECK(track_container->has_track(track_id));

    BOOST_CHECK_NO_THROW(ret = track_container->split_track(track_id, 4));
    BOOST_CHECK(!ret);
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 1);
    BOOST_CHECK(track_container->has_track(track_id));

    BOOST_CHECK_NO_THROW(ret = track_container->split_track(track_id, 2));
    BOOST_CHECK(ret);
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 2);
    BOOST_CHECK(track_container->has_track(track_id));
    BOOST_CHECK(track_container->has_track(track_id + 1));

    {
        std::unique_ptr<Track> track;
        size_t current_track_id = track_id;
        BOOST_CHECK(track_container->has_track(current_track_id));
        BOOST_CHECK_NO_THROW(track = track_container->get_track(current_track_id));

        size_t frame_idx = 1;
        for (const auto& det : *track) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            frame_idx++;
        }
    }

    {
        std::unique_ptr<Track> track;
        size_t current_track_id = track_id + 1;
        BOOST_CHECK(track_container->has_track(current_track_id));
        BOOST_CHECK_NO_THROW(track = track_container->get_track(current_track_id));

        size_t frame_idx = 2;
        for (const auto& det : *track) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            frame_idx++;
        }
    }

}

BOOST_AUTO_TEST_CASE(track_operations)
{
    size_t video_len = 4;
    std::unique_ptr<TrackContainer> track_container;
    BOOST_CHECK_NO_THROW(track_container = std::make_unique<TrackContainer>(video_len));

    {
        {
            size_t track_id = 2;
            Track track(track_id);
            Detection det;
            det.id = track_id;

            det.frame = 0;
            track.push_back(det);

            det.frame = 3;
            track.push_back(det);

            BOOST_CHECK_NO_THROW(track_container->add_track(track));
        }

        {
            size_t track_id = 5;
            Track track(track_id);
            Detection det;
            det.id = track_id;

            det.frame = 1;
            track.push_back(det);

            det.frame = 2;
            track.push_back(det);

            BOOST_CHECK_NO_THROW(track_container->add_track(track));
        }
    }

    BOOST_CHECK_EQUAL(track_container->get_video_len(), video_len);
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 2);

    {
        {
            std::unique_ptr<Track> track;
            size_t track_id = 2;
            BOOST_CHECK(track_container->has_track(track_id));
            BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));

            size_t frame_idx = 0;
            for (const auto& det : *track) {
                BOOST_CHECK_EQUAL(det.frame, frame_idx);
                frame_idx += 3;
            }
        }
        {
            std::unique_ptr<Track> track;
            size_t track_id = 5;
            BOOST_CHECK(track_container->has_track(track_id));
            BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));

            size_t frame_idx = 1;
            for (const auto& det : *track) {
                BOOST_CHECK_EQUAL(det.frame, frame_idx);
                frame_idx += 1;
                if (frame_idx >= 3) {
                    break;
                }
            }
        }
    }

    BOOST_CHECK(track_container->has_track(2));
    BOOST_CHECK(track_container->has_track(5));

    bool ret;
    BOOST_CHECK_NO_THROW(ret = track_container->delete_track(5));
    BOOST_CHECK(ret);

    BOOST_CHECK(track_container->has_track(2));
    BOOST_CHECK(!track_container->has_track(5));
    BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 1);

    {
        std::unique_ptr<Track> track;
        size_t track_id = 2;
        BOOST_CHECK(track_container->has_track(track_id));
        BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));

        size_t frame_idx = 0;
        for (const auto& det : *track) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            frame_idx += 3;
        }
    }

    {
        {
            size_t track_id = 5;
            Track track(track_id);
            Detection det;
            det.id = track_id;

            det.frame = 1;
            track.push_back(det);

            det.frame = 2;
            track.push_back(det);

            det.frame = 3;
            track.push_back(det);

            BOOST_CHECK_NO_THROW(track_container->add_track(track));
        }

        BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 2);

        track_container->unite_tracks(2, 5);

        BOOST_CHECK_EQUAL(track_container->get_num_tracks(), 1);
        BOOST_CHECK(track_container->has_track(2));
        BOOST_CHECK(!track_container->has_track(5));

        {
            std::unique_ptr<Track> track;
            size_t track_id = 2;
            BOOST_CHECK(track_container->has_track(track_id));
            BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));

            size_t frame_idx = 0;
            for (const auto& det : *track) {
                BOOST_CHECK_EQUAL(det.frame, frame_idx);
                frame_idx += 1;
            }

            BOOST_CHECK_EQUAL(frame_idx, video_len);
        }
    }

    {
        size_t track_id = 2;
        BOOST_CHECK(track_container->has_track(track_id));
        BOOST_CHECK_EQUAL(track_container->get_video_len(), video_len);
        for (size_t i = 0; i < track_container->get_video_len(); i += 2) {
            BOOST_CHECK(track_container->delete_detection(track_id, i));
        }

        std::unique_ptr<Track> track;
        BOOST_CHECK(track_container->has_track(track_id));
        BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));
        BOOST_CHECK(track != nullptr);

        size_t frame_idx = 1;
        for (const auto& det : *track) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            frame_idx += 2;
        }
        BOOST_CHECK_EQUAL(frame_idx - 2, 3);

        BOOST_CHECK(track_container->has_track(track_id));
        BOOST_CHECK_EQUAL(track_container->get_video_len(), video_len);
        for (size_t i = 0; i < track_container->get_video_len(); i += 2) {
            Detection det;
            det.id = track_id;
            det.frame = i;
            BOOST_CHECK(track_container->add_det2track(track_id, det));
        }

        BOOST_CHECK(track_container->has_track(track_id));
        BOOST_CHECK_NO_THROW(track = track_container->get_track(track_id));
        BOOST_CHECK(track != nullptr);
        frame_idx = 0;
        for (const auto& det : *track) {
            BOOST_CHECK_EQUAL(det.frame, frame_idx);
            frame_idx++;
        }
        BOOST_CHECK_EQUAL(frame_idx, video_len);
    }
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
