#pragma once

#include <opencv2/core/core.hpp>
#include <assert.h>

/*template<typename ValueType>
class OwnIterator: public std::iterator<std::input_iterator_tag, ValueType>
{
    friend class OwnContainer;
private:
    OwnIterator(ValueType* p);
public:
    OwnIterator(const OwnIterator &it);

    bool operator!=(OwnIterator const& other) const;
    bool operator==(OwnIterator const& other) const; //need for BOOST_FOREACH
    typename OwnIterator::reference operator*() const;
    OwnIterator& operator++();
private:
    ValueType* p;
};*/

class Video {
public:
    Video() = delete;
    explicit Video(const std::string& dir_path, size_t fps=25);

    const std::vector<std::string>& get_frame_names() const {
        return sequence_;
    }

    cv::Mat get_frame(size_t index) const;

    size_t size() const {
        return sequence_.size();
    }

    // TODO: frame.size()

    // Make iterators
private:
    std::vector<std::string> sequence_;
    std::string dir_path_;
    size_t fps_;
};
