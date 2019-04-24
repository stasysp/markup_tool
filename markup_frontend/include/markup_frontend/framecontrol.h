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
    QPushButton *btn1 = nullptr;
    QPushButton *btn2 = nullptr;
    QPushButton *btn3 = nullptr;
    QPushButton *btn4 = nullptr;
    QPushButton *btn5 = nullptr;
    QPushButton *btn6 = nullptr;
    QPushButton *btn7 = nullptr;
    TimeLineControl *timeline = nullptr;
};

#endif // FRAMECONTROL_H
