#ifndef FRAMEWITHCONTROL_H
#define FRAMEWITHCONTROL_H

#include <QWidget>
#include <QDir>

#include "frameview.h"
#include "framecontrol.h"

class FrameWithControl : public QWidget
{
    Q_OBJECT
public:
    explicit FrameWithControl(QWidget *parent = nullptr);
    int getFrameIdx() const { return frameidx; }
    int getTrackOnFocus() const { return frameview->getTrackOnFocus(); }

signals:
    void send_framechanged(FrameWithControl* fwc);
    void send_delete_bbox(int track_id, int frameidx);
    void send_delete_track(int track_id);
    void send_split_track(int track_id, int frameidx);
    void send_unite_tracks();

public slots:
    void slot_delete_bbox();
    void slot_delete_track();
    void slot_split_track();
    void slot_unite_tracks();
    void setFrameIdx(int idx);
    void addFrameIdx(int move);
    void doOnFrameChange();
    void setPath(QDir path);
    void setMarkup(QMap<int,ScaledBBox> markup);
    void update();

private:
    void reset();

private:
    int frameidx = 0;
    int n_frames = 0;
    QDir img_path;

    FrameView *frameview = nullptr;
    FrameControl *framecontrol = nullptr;
};

#endif // FRAMEWITHCONTROL_H
