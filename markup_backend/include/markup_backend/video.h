#pragma once

#include <opencv2/core.hpp>
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
    explicit Video(const std::string& dir_path);

    const std::vector<std::string>& get_frame_names() const {
        return sequence;
    }

    cv::Mat get_frame(size_t index) const;

    size_t size() const {
        return sequence.size();
    }

    // iterators begin end
    // get_frame(index)
private:
    std::vector<std::string> sequence;

    size_t fps;
};
