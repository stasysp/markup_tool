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
    PipelineRunParams params;
    params.tracker_model_path = "../../../markup_tool/python/detector/yolo3.py";
    std::string test_video_dir = "../../../markup_tool/data/test/MOT16-04/img1/";

    std::unique_ptr<MarkUp> mark_up;
    BOOST_CHECK_NO_THROW(mark_up = std::make_unique<MarkUp>(params));

    std::unique_ptr<Video> video;
    BOOST_CHECK_NO_THROW(video = std::make_unique<Video>(test_video_dir));

    std::unique_ptr<TrackContainer> track_container;
    // BOOST_CHECK_NO_THROW(track_container = mark_up->run(*video));
}

BOOST_AUTO_TEST_CASE(tracks)
{
    std::unique_ptr<Track> track;
    BOOST_CHECK_NO_THROW(track = std::make_unique<Track>(42));

    Detection det;
    det.frame = 0;
    det.id = 42;

    BOOST_CHECK_NO_THROW(track->push_back(det));
    std::unique_ptr<Detection> det_ptr;
    BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(1));
    BOOST_CHECK_EQUAL(det_ptr, nullptr);

    for (size_t i = 0; i < 10; ++i) {
        BOOST_CHECK_NO_THROW(det_ptr = track->get_detection(0));
        BOOST_CHECK(det_ptr != nullptr);

        BOOST_CHECK_EQUAL(det_ptr->frame, 0);
        BOOST_CHECK_EQUAL(det_ptr->id, det.id);
    }


}

BOOST_AUTO_TEST_SUITE_END()

