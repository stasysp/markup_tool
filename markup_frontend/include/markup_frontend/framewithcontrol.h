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
    void send_framechanged(FrameWithControl* fwc);

public slots:
    void setFrameIdx(int idx);
    void addFrameIdx(int move);
    void doOnFrameChange();
    void setPath(QDir path);

    void setMarkup(QMap<int,ScaledBBox> markup);

    void testdebug();

private:
    void reset();

private:
    int frameidx = 0;
    int n_frames = 0;
    QDir img_path;

    FrameView *frameview = nullptr;
    FrameControl *framecontrol = nullptr;
};

#endif // FRAMEWITHCONTROL_H
