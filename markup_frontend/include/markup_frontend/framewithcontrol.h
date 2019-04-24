#ifndef FRAMEWITHCONTROL_H
#define FRAMEWITHCONTROL_H

#include <QWidget>
#include <QDir>

#include "frameview.h"
#include "framecontrol.h"

class FrameWithControl : public QWidget
{
    Q_OBJECT
public:
    explicit FrameWithControl(QWidget *parent = nullptr);
    int getFrameIdx() const { return frameidx; }

    signals:
            void send_framechanged(int frameidx);

public slots:
            void setFrameIdx(int idx);
    void addFrameIdx(int move);
    void doOnFrameChange();

private:
    // вообще-то эта фигня должна быть optional...
    int frameidx = 0;
    // пока стоит как заглушка...
    // нужно определять по числу кардов в видео...
    int n_frames = 100;
    QDir path;

    FrameView *frameview = nullptr;
    FrameControl *framecontrol = nullptr;
};

#endif // FRAMEWITHCONTROL_H
