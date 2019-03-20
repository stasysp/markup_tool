#pragma once

#include <iterator>
#include <string>
#include <vector>

using std::vector;
using std::string;

struct TPoint {
    int X;
    int Y;
};

struct TBoundingBox {
    TPoint LowerLeft;
    TPoint UpperRight;
    int FrameNumber;

    TPoint Center() const {
        return {
            (LowerLeft.X + UpperRight.X) / 2,
            (LowerLeft.Y + UpperRight.Y) / 2
        };
    }

    int Width() const {
        return UpperRight.X - LowerLeft.X;
    }

    int Height() const {
        return UpperRight.Y - LowerLeft.Y;
    }
};

class TTrack {
public:
    TTrack(const string& name, const TBoundingBox& initBox);

    vector<TBoundingBox> Get() const;
    TBoundingBox Get(const int idx) const;
    vector<TBoundingBox>::const_iterator begin() const;
    vector<TBoundingBox>::const_iterator end() const;

    vector<TTrack> DivideByFrame(int frameNumber) const;
    TBoundingBox GetByFrame(int frameNumber) const;

    void AddBox(TBoundingBox box);
    void DeleteBox(int frameNumber);
    void ConnectWith(TTrack other);
    void ConnectWith(TTrack&& other);
private:
    std::pair<TBoundingBox, TBoundingBox> FindFrameBounds(int frameNumber);

    string Name;
    vector<TBoundingBox> TrackItems;
};

