// #define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include "opencv2/core/core.hpp"

#include "markup_backend/utils.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(utils_suit)

BOOST_AUTO_TEST_CASE(read_det_from_file)
        {

                std::string det_filepath = "../../../markup_tool/data/test/MOT16-04/gt/gt.txt";

                std::unique_ptr<TrackContainer> detected_tracks;

                BOOST_CHECK_NO_THROW(detected_tracks = read_dummy_trackcontainer(det_filepath));

                BOOST_CHECK_EQUAL(detected_tracks->get_video_len(), 5);

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
                size_t frame_index = 0;
                BOOST_CHECK_NO_THROW(frame_detections = detected_tracks->get_detections(frame_index));
                BOOST_CHECK(!frame_detections.empty());
                for (const auto& detection : frame_detections) {
                        BOOST_CHECK_EQUAL(detection.frame, frame_index);
                }

        }

BOOST_AUTO_TEST_SUITE_END()
