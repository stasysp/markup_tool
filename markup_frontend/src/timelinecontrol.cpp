#include <QLabel>

#include "markup_frontend/timelinecontrol.h"

TimeLineControl::TimeLineControl(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(this);
    label->setText("TimelinePlaceHolder");
}
