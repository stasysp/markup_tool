#ifndef TIMELINECONTROL_H
#define TIMELINECONTROL_H

#include <QWidget>
#include <QSlider>
#include <QAbstractButton>
#include <QLCDNumber>

class TimeLineControl : public QWidget
{
    Q_OBJECT
public:
    explicit TimeLineControl(QWidget *parent = nullptr);

signals:
    void send_frameidx(int fnumber);

private slots:
    void slot_onesecondback();
    void slot_oneframeback();
    void slot_oneframeforward();
    void slot_onesecondforward();
    void slot_sliderchanged(int frameidx);

public slots:
    void setTimelineProperties(int frameidx, int nframes);

private:
    size_t framespersecond = 50; // set on video load...

    QSlider *slider = nullptr;
    QAbstractButton *onesecondback = nullptr;
    QAbstractButton *oneframeback = nullptr;
    QLCDNumber *currentframenumber = nullptr;
    QAbstractButton *oneframeforward = nullptr;
    QAbstractButton *onesecondforward = nullptr;
};

#endif // TIMELINECONTROL_H
