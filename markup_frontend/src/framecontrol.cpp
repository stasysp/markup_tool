#include <QVBoxLayout>

#include "markup_frontend/framecontrol.h"
#include "markup_frontend/timelinecontrol.h"

FrameControl::FrameControl(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    btn1 = new QPushButton("Add new bounding box");
    btn2 = new QPushButton("Select bounding box");
    btn3 = new QPushButton("Delete bounding box");
    btn4 = new QPushButton("Select track");
    btn5 = new QPushButton("Delete track");
    btn6 = new QPushButton("Unite tracks");
    btn7 = new QPushButton("Split track");

    timeline = new TimeLineControl();

    vlayout->addWidget(btn1);
    vlayout->addWidget(btn2);
    vlayout->addWidget(btn3);
    vlayout->addWidget(btn4);
    vlayout->addWidget(btn5);
    vlayout->addWidget(btn6);
    vlayout->addWidget(btn7);
    vlayout->addWidget(timeline);
}
