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
                std::string det_filepath = "../../../markup_tool/data/test/MOT16-04/det/det.txt";

                std::vector<std::vector<Detection>> detections;

                BOOST_CHECK_NO_THROW(detections = read_mot16_detections(det_filepath));

                BOOST_CHECK_EQUAL(detections.size(), 5);
        }

BOOST_AUTO_TEST_SUITE_END()