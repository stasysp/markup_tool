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
    void slot_mode(bool isSelectMode);

public:
    QPushButton *select = nullptr;
    QPushButton *btn_add_bbox = nullptr;
    QPushButton *btn_del_bbox = nullptr;
    QPushButton *btn_del_track = nullptr;
    QPushButton *btn_unt_track = nullptr;
    QPushButton *btn_spl_track = nullptr;
    QPushButton *btn_interpolate = nullptr;
    TimeLineControl *timeline = nullptr;

private:
    QString getStyleSheet(bool flag);
};

#endif // FRAMECONTROL_H
