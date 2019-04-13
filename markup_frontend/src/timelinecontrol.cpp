#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStyle>

#include "markup_frontend/timelinecontrol.h"
#include "markup_frontend/framecontrol.h"

TimeLineControl::TimeLineControl(QWidget *parent) : QWidget(parent)
{
    slider = new QSlider(Qt::Horizontal, this);

    onesecondback = new QToolButton(this);
    onesecondback->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    oneframeback = new QToolButton(this);
    oneframeback->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    oneframeforward = new QToolButton(this);
    oneframeforward->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    onesecondforward = new QToolButton(this);
    onesecondforward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

    currentframenumber = new QLCDNumber(this);
    currentframenumber->setDigitCount(6);

    setTimelineProperties(0, 100);

    QVBoxLayout *vlayout = new QVBoxLayout(this);

    QHBoxLayout* buttonslayout = new QHBoxLayout;
    buttonslayout->addWidget(onesecondback);
    buttonslayout->addWidget(oneframeback);
    buttonslayout->addWidget(currentframenumber);
    buttonslayout->addWidget(oneframeforward);
    buttonslayout->addWidget(onesecondforward);

    vlayout->addWidget(slider);
    vlayout->addLayout(buttonslayout);

    connect(onesecondback, &QAbstractButton::clicked, this, &TimeLineControl::slot_onesecondback);
    connect(oneframeback, &QAbstractButton::clicked, this, &TimeLineControl::slot_oneframeback);
    connect(oneframeforward, &QAbstractButton::clicked, this, &TimeLineControl::slot_oneframeforward);
    connect(onesecondforward, &QAbstractButton::clicked, this, &TimeLineControl::slot_onesecondforward);

    connect(slider, &QSlider::valueChanged, this, &TimeLineControl::slot_sliderchanged);
}

void TimeLineControl::slot_onesecondback() { emit send_frameidx(slider->value()-framespersecond); }
void TimeLineControl::slot_oneframeback() { emit send_frameidx(slider->value()-1); }
void TimeLineControl::slot_oneframeforward() { emit send_frameidx(slider->value()+1); }
void TimeLineControl::slot_onesecondforward() { emit send_frameidx(slider->value()+framespersecond); }
void TimeLineControl::slot_sliderchanged(int frameidx) { emit send_frameidx(frameidx); }

void TimeLineControl::setTimelineProperties(int frameidx, int nframes) {
    slider->setMaximum(nframes-1);
    slider->setValue(frameidx);
    currentframenumber->display(frameidx);
}


