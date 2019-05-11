// #define BOOST_TEST_MODULE BackendTests

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>

#include "markup_backend/video.h"

using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(video_suit)

BOOST_AUTO_TEST_CASE( constructors ) {
    std::string test_video_dir = "../../../markup_tool/data/test/MOT16-04/img1/";
    std::string first_img_filename = "000001.jpg";
    std::string last_img_filename = "000050.jpg";
    size_t num_frames = 50;
    cv::Size img_size(1920, 1080);

    std::unique_ptr<Video> video;
    BOOST_CHECK_NO_THROW(video = std::make_unique<Video>(test_video_dir));

    std::vector<std::string> found_filenames;
    BOOST_CHECK_NO_THROW(found_filenames = video->get_frame_names());
    BOOST_CHECK_EQUAL(found_filenames.size(), num_frames);

    BOOST_CHECK_EQUAL(found_filenames.front(), first_img_filename);
    BOOST_CHECK_EQUAL(found_filenames.back(), last_img_filename);

    BOOST_CHECK_EQUAL(video->size(), num_frames);

    for (size_t frame_index = 0; frame_index < video->size(); ++frame_index) {
        cv::Mat img;
        BOOST_CHECK_NO_THROW(img = video->get_frame(frame_index));
        BOOST_CHECK(!img.empty());
        BOOST_CHECK(img.size() == img_size);
    }

    // Doesnt work on Linux
    // BOOST_CHECK(cv::countNonZero(video->get_frame(0) == video->get_frame(1)) != 0);
}

BOOST_AUTO_TEST_SUITE_END()
