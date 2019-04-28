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

    connect(maincontrol, &MainControlPanel::send_video_path, fwcup, &FrameWithControl::setPath);
    connect(maincontrol, &MainControlPanel::send_video_path, fwcdn, &FrameWithControl::setPath);
    connect(maincontrol, &MainControlPanel::send_video_path, this, &MarkupWidget::slot_set_video_path);

    connect(maincontrol, &MainControlPanel::send_tracks_path, this, &MarkupWidget::slot_set_tracks_path);

    connect(maincontrol, &MainControlPanel::send_run, this, &MarkupWidget::slot_run);

    connect(fwcup, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);
    connect(fwcup, &FrameWithControl::send_delete_bbox, this, &MarkupWidget::slot_delete_bbox);
    connect(fwcup, &FrameWithControl::send_delete_track, this, &MarkupWidget::slot_delete_track);

    connect(fwcdn, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);
    connect(fwcdn, &FrameWithControl::send_delete_bbox, this, &MarkupWidget::slot_delete_bbox);
    connect(fwcdn, &FrameWithControl::send_delete_track, this, &MarkupWidget::slot_delete_track);
}

void MarkupWidget::slot_set_video_path(QDir path) {
    markup.set_video(std::string(path.path().toUtf8().constData()));
    path = path;
}

void MarkupWidget::slot_set_tracks_path(QString path) {
    markup.set_tracks(std::string(path.toUtf8().constData()));
    path = path;
}

void MarkupWidget::slot_run() {
    qDebug() << "Rewrite using new MarkUp interface";
    markup.run();
    update();
    qDebug() << "run finished...";
}

void MarkupWidget::slot_framechanged(FrameWithControl *fwc) {
    int frameidx = fwc->getFrameIdx();
    QMap<int, ScaledBBox> bboxes;
    std::vector<Detection> detections;
    // qDebug() << fwc << "slot_framechanged" << frameidx;

    bool res = markup.get_frame(frameidx, &detections);

    if (res) {
        // qDebug() << "get frame is succesfull...";
        for (auto det : detections) {
            bboxes[det.id] = ScaledBBox(det);
        }
        fwc->setMarkup(bboxes);
    } else {
        // qDebug() << "get frame is not succesfull...";
        fwc->setMarkup(bboxes);
    }
}

void MarkupWidget::slot_delete_bbox(int track_id, int frameidx) {
    markup.delete_detection(track_id, frameidx);
    update();
    // qDebug() << "delete bbox..." << track_id << frameidx;
}

void MarkupWidget::slot_delete_track(int track_id) {
    markup.delete_track(track_id);
    update();
    // qDebug() << "delete track..." << track_id;
}

void MarkupWidget::update() {
    fwcup->update();
    fwcdn->update();
}
