#include <QVBoxLayout>

#include "markup_frontend/framecontrol.h"
#include "markup_frontend/timelinecontrol.h"

FrameControl::FrameControl(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);

    select = new QPushButton("Select");
    select->setStyleSheet(getStyleSheet(true));
    btn_add_bbox = new QPushButton("Add new bounding box");
    btn_del_bbox = new QPushButton("Delete bounding box");
    btn_del_track = new QPushButton("Delete track");
    btn_unt_track = new QPushButton("Unite tracks");
    btn_spl_track = new QPushButton("Split track");
    btn_interpolate = new QPushButton("Interpolate");

    timeline = new TimeLineControl();

    vlayout->addWidget(select);
    vlayout->addWidget(btn_add_bbox);
    vlayout->addStretch(1);
    vlayout->addWidget(btn_del_bbox);
    vlayout->addWidget(btn_del_track);
    vlayout->addWidget(btn_unt_track);
    vlayout->addWidget(btn_spl_track);
    vlayout->addWidget(btn_interpolate);
    vlayout->addStretch(2);
    vlayout->addWidget(timeline);
    timeline->setTimelineProperties(0,0);

    connect(timeline, &TimeLineControl::send_frameidx, this, &FrameControl::slot_setnewframeidx);
}

void FrameControl::slot_setnewframeidx(int frameidx) {
    emit send_newframeidx(frameidx);
}

void FrameControl::slot_mode(bool isSelectMode) {
    select->setStyleSheet(getStyleSheet(isSelectMode));
    btn_add_bbox->setStyleSheet(getStyleSheet(!isSelectMode));
}

QString FrameControl::getStyleSheet(bool flag) {
    if (flag) {
        return "QPushButton {background-color: #A3C1DA}";
    } else {
        return "QPushButton {}";
    }
}
