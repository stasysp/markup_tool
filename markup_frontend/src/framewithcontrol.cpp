#include <QHBoxLayout>

#include "markup_frontend/framewithcontrol.h"


FrameWithControl::FrameWithControl(QWidget *parent) : QWidget(parent)
{
    frameview = new FrameView(this);
    framecontrol = new FrameControl(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(frameview);
    hlayout->addWidget(framecontrol);

    connect(framecontrol, &FrameControl::send_newframeidx,
            this, &FrameWithControl::setFrameIdx);
}

void FrameWithControl::setFrameIdx(int idx) {
    if (n_frames <= idx) { idx = n_frames - 1; }
    if (idx < 0) { idx = 0; }
    if (frameidx != idx) {
        frameidx = idx;
        doOnFrameChange();
    }
}

void FrameWithControl::addFrameIdx(int move) {
    setFrameIdx(frameidx + move);
}

void FrameWithControl::doOnFrameChange() {
    framecontrol->timeline->setTimelineProperties(frameidx, n_frames);
}
