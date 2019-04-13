#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStyle>

#include "markup_frontend/timelinecontrol.h"

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
    currentframenumber->display(0);

    setNumberOfFrames(1);

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

    connect(this, &TimeLineControl::send_framechanged, this, &TimeLineControl::slot_setsliderposition);
    connect(slider, &QSlider::valueChanged, this, &TimeLineControl::slot_setlcdvalue);
}

void TimeLineControl::trymoveframe(int nframes) {
    int tryframe = currentframenumber->value() + nframes;
    if (tryframe < 0) tryframe = 0;
    if (tryframe >= numberofframes) tryframe = numberofframes - 1;
    currentframenumber->display(tryframe);

    emit send_framechanged();
}

void TimeLineControl::slot_onesecondback() { trymoveframe(-framespersecond); }
void TimeLineControl::slot_oneframeback() { trymoveframe(-1); }
void TimeLineControl::slot_oneframeforward() { trymoveframe(1); }
void TimeLineControl::slot_onesecondforward() { trymoveframe(framespersecond); }

void TimeLineControl::slot_setsliderposition() {
    slider->setValue(currentframenumber->value());
}

void TimeLineControl::slot_setlcdvalue() {
    currentframenumber->display(slider->value());
}

void TimeLineControl::setNumberOfFrames(int nframes) {
    numberofframes = nframes;
    slider->setMaximum(numberofframes-1);
}

