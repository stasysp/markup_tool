#define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>

#include "markup_backend/video.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(video_suit)

BOOST_AUTO_TEST_CASE( constructors )
{
    std::string test_video_dir = "../../../markup_tool/data/test/MOT16-04/img1/";

    std::unique_ptr<Video> video;
    BOOST_CHECK_NO_THROW(video = std::make_unique<Video>(test_video_dir));

    std::vector<std::string> found_filenames;
    BOOST_CHECK_NO_THROW(found_filenames = video->get_frame_names());

    BOOST_CHECK_EQUAL(found_filenames.front(), "000001.jpg");
    BOOST_CHECK_EQUAL(found_filenames.back(), "000005.jpg");
}

BOOST_AUTO_TEST_SUITE_END()
