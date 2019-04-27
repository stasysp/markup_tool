#include <QVBoxLayout>

#include "markup_frontend/framecontrol.h"
#include "markup_frontend/timelinecontrol.h"

FrameControl::FrameControl(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);

    btn_add_bbox = new QPushButton("Add new bounding box");
    btn_sel_bbox = new QPushButton("Select bounding box");
    btn_del_bbox = new QPushButton("Delete bounding box");
    btn_sel_track = new QPushButton("Select track");
    btn_del_track = new QPushButton("Delete track");
    btn_unt_track = new QPushButton("Unite tracks");
    btn_spl_track = new QPushButton("Split track");

    timeline = new TimeLineControl();

    vlayout->addWidget(btn_add_bbox);
    vlayout->addWidget(btn_sel_bbox);
    vlayout->addWidget(btn_del_bbox);
    vlayout->addWidget(btn_sel_track);
    vlayout->addWidget(btn_del_track);
    vlayout->addWidget(btn_unt_track);
    vlayout->addWidget(btn_spl_track);
    vlayout->addWidget(timeline);
    timeline->setTimelineProperties(0,0);

    connect(timeline, &TimeLineControl::send_frameidx, this, &FrameControl::slot_setnewframeidx);
}

void FrameControl::slot_setnewframeidx(int frameidx) {
    emit send_newframeidx(frameidx);
}

