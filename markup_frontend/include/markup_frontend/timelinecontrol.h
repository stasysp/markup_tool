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
    TimeLineControl(QWidget *parent = nullptr);

    void setNumberOfFrames(int nframes);

private:
    void trymoveframe(int nframes);

    signals:
            void send_framenumber(int fnumber);
    void send_framechanged();

public slots:
            void slot_onesecondback();
    void slot_oneframeback();
    void slot_oneframeforward();
    void slot_onesecondforward();

private slots:
            void slot_setsliderposition();
    void slot_setlcdvalue();

private:
    size_t numberofframes = 5; // set on video load...
    size_t framespersecond = 50; // set on video load...

    QSlider *slider = nullptr;
    QAbstractButton *onesecondback = nullptr;
    QAbstractButton *oneframeback = nullptr;
    QLCDNumber *currentframenumber = nullptr;
    QAbstractButton *oneframeforward = nullptr;
    QAbstractButton *onesecondforward = nullptr;
};

#endif // TIMELINECONTROL_H