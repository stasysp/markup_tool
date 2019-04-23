#include "markup_frontend/markupwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

MarkupWidget::MarkupWidget(QWidget *parent)
        : QWidget(parent)
{
    maincontrol = new MainControlPanel(this);
    fwcup = new FrameWithControl(this);
    fwcdn = new FrameWithControl(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QVBoxLayout* fwclayout = new QVBoxLayout;

    hlayout->addWidget(maincontrol);
    hlayout->addLayout(fwclayout);

    fwclayout->addWidget(fwcup);
    fwclayout->addWidget(fwcdn);

    connect(maincontrol, &MainControlPanel::send_path, fwcup, &FrameWithControl::setPath);
    connect(maincontrol, &MainControlPanel::send_path, fwcdn, &FrameWithControl::setPath);
    connect(maincontrol, &MainControlPanel::send_path, this, &MarkupWidget::slot_set_video_path);

    connect(maincontrol, &MainControlPanel::send_run, this, &MarkupWidget::slot_run);

    connect(fwcup, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);
    connect(fwcdn, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);

    params = new PipelineRunParams();
}

// тут нужно передавать путь в бекэнд... но сейчас некуда...
void MarkupWidget::slot_set_video_path(QDir path) {
    path = path;
}

void MarkupWidget::slot_run() {
    markup = new MarkUp(*params);
    std::string str = std::string(path.path().toUtf8().constData());
    Video video(str);
    trackcontainer = markup->run(video);
    qDebug() << "run finished...";
}

void MarkupWidget::slot_framechanged(FrameWithControl *fwc) {
    qDebug() << fwc;
    int frameidx = fwc->getFrameIdx();
    QMap<int, ScaledBBox> bboxes;
    qDebug() << "slot_framechanged";
    for (auto det : trackcontainer->get_detections(frameidx)) {
        bboxes[det.id] = ScaledBBox(det);
        //qDebug() << det.id;
    }
    qDebug() << bboxes.size();
    fwc->testdebug();
    fwc->setMarkup(bboxes);
    //update();
}
