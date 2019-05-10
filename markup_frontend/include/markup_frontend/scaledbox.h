#ifndef SCALEDBOX_H
#define SCALEDBOX_H

#include <QWidget>
#include <QPoint>
#include <QRect>
#include <QVector>

#include "markup_backend/definitions.h"

class TrackLine {
public:
    explicit TrackLine() {}
    explicit TrackLine(QVector<QPoint> track, int frameIdx = 0) :
        _track(std::move(track)), _trackFirstFrameIdx(frameIdx) {}

    int getFirstFrameIdx() const { return _trackFirstFrameIdx; }
    int getLastFrameIdx() const { return _trackFirstFrameIdx + _track.size() - 1; }

    // плохо. нужно отдавать константную ссылку
    QVector<QPoint> getTrackLine() const { return _track; }
    void setTrackLine(QVector<QPoint> track, int frameIdx = 0) {
        _track = std::move(track);
        _trackFirstFrameIdx = frameIdx;
    }

private:
    QVector<QPoint> _track;
    int _trackFirstFrameIdx;
};

class ScaledBBox
{
public:
    explicit ScaledBBox(int left=0, int top=0, int right=1920, int bottom=1080, int width=1920, int height=1080);
    explicit ScaledBBox(QPoint topleft, QPoint bottomright, int width=1920, int height=1080);
    explicit ScaledBBox(const Detection& det, int width=1920, int height=1080);
    explicit ScaledBBox(const DetectionAndTrack& det, int width=1920, int height=1080);

    QRect getScaledRect() const;

    bool isInside(int x, int y) const;

    int getFirstFrameIdx() const { return track.getFirstFrameIdx(); }
    int getLastFrameIdx() const { return track.getLastFrameIdx(); }
    QVector<QPoint> getTrackLine() const { return track.getTrackLine(); }
    void setTrackLine(QVector<QPoint> track, int frameIdx = 0) {
        this->track.setTrackLine(track, frameIdx);
    }

private:
    void _checkSize();

private:
    int _width;
    int _height;
    QRect bbox;
    TrackLine track;
};

#endif // SCALEDBOX_H
