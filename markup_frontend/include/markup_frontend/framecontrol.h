#ifndef FRAMECONTROL_H
#define FRAMECONTROL_H

#include <QPushButton>

#include "timelinecontrol.h"


class FrameControl : public QWidget
{
    Q_OBJECT
public:
    explicit FrameControl(QWidget *parent = nullptr);

    signals:
            void send_newframeidx(int frameidx);

public slots:
            void slot_setnewframeidx(int frameidx);

public:
    QPushButton *btn_add_bbox = nullptr;
    QPushButton *btn_sel_bbox = nullptr;
    QPushButton *btn_del_bbox = nullptr;
    QPushButton *btn_sel_track = nullptr;
    QPushButton *btn_del_track = nullptr;
    QPushButton *btn_unt_track = nullptr;
    QPushButton *btn_spl_track = nullptr;
    TimeLineControl *timeline = nullptr;
};

#endif // FRAMECONTROL_H
