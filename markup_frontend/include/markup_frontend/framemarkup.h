#ifndef FRAMEMARKUP_H
#define FRAMEMARKUP_H

#include <QMap>
#include "scaledbox.h"

class FrameMarkup
{
public:
    FrameMarkup();

    QMap<int, ScaledBBox>::const_iterator begin() const;
    QMap<int, ScaledBBox>::const_iterator end() const;

    void set_new_markup(QMap<int, ScaledBBox> newmarkup);

    void add(int track_id, ScaledBBox bbox);
    void remove(int track_id);
    void clear();
    int select_bbox(int x, int y);

private:
    QMap<int, ScaledBBox> bboxes;
};

#endif // FRAMEMARKUP_H
