#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>

#include "maincontrolpanel.h"
#include "framewithcontrol.h"

#include "markup_backend/markup.h"
#include "markup_backend/tracks.h"

class MarkupWidget : public QWidget
{
Q_OBJECT

public:
    MarkupWidget(QWidget *parent = nullptr);

public slots:
    void slot_set_video_path(QDir path);
    void slot_set_tracks_path(QString path);
    void slot_savetracks(QString path);
    void slot_framechanged(FrameWithControl *fwc);
    void slot_run();
    void slot_delete_bbox(int track_id, int frameidx);
    void slot_delete_track(int track_id);
    void slot_split_track(int track_id, int frameidx);
    void slot_unite_tracks();

private:
    MainControlPanel *maincontrol = nullptr;
    FrameWithControl *fwcup = nullptr;
    FrameWithControl *fwcdn = nullptr;
    MarkUp markup;

private:
    void update();
};

#endif // WIDGET_H
