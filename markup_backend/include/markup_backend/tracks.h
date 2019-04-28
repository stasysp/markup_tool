#pragma once

#include <list>
#include <memory>
#include <set>

#include <iostream>

#include "markup_backend/definitions.h"

class Track {
public:
    Track() = delete;
    explicit Track(size_t id) : id_(id) {}

    Detection* push_back(const Detection& det);
    Detection* add(const Detection& det);
    void del(size_t frame_idx);
    size_t get_id() const;

    std::list<Detection>::iterator begin();
    std::list<Detection>::iterator end();

    std::list<Detection>::const_iterator begin() const;
    std::list<Detection>::const_iterator end() const;

    std::unique_ptr<Detection> get_detection(size_t frame_idx);
    std::unique_ptr<Detection> get_last_detection() {
        if (detections_.empty()) {
            return nullptr;
        }

        return std::make_unique<Detection>(detections_.back());
    }

private:
    std::list<Detection> detections_;
    size_t id_;
};

