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
    connect(maincontrol, &MainControlPanel::send_savetracks, this, &MarkupWidget::slot_savetracks);
    connect(maincontrol, &MainControlPanel::send_run, this, &MarkupWidget::slot_run);

    connect(fwcup, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);
    connect(fwcup, &FrameWithControl::send_add_bbox, this, &MarkupWidget::slot_add_bbox);
    connect(fwcup, &FrameWithControl::send_delete_bbox, this, &MarkupWidget::slot_delete_bbox);
    connect(fwcup, &FrameWithControl::send_delete_track, this, &MarkupWidget::slot_delete_track);
    connect(fwcup, &FrameWithControl::send_split_track, this, &MarkupWidget::slot_split_track);
    connect(fwcup, &FrameWithControl::send_unite_tracks, this, &MarkupWidget::slot_unite_tracks);

    connect(fwcdn, &FrameWithControl::send_framechanged, this, &MarkupWidget::slot_framechanged);
    connect(fwcdn, &FrameWithControl::send_add_bbox, this, &MarkupWidget::slot_add_bbox);
    connect(fwcdn, &FrameWithControl::send_delete_bbox, this, &MarkupWidget::slot_delete_bbox);
    connect(fwcdn, &FrameWithControl::send_delete_track, this, &MarkupWidget::slot_delete_track);
    connect(fwcdn, &FrameWithControl::send_split_track, this, &MarkupWidget::slot_split_track);
    connect(fwcdn, &FrameWithControl::send_unite_tracks, this, &MarkupWidget::slot_unite_tracks);
}

void MarkupWidget::slot_set_video_path(QDir path) {
    markup.set_video(std::string(path.path().toUtf8().constData()));
    path = path;
    update();
}

void MarkupWidget::slot_set_tracks_path(QString path) {
    markup.load_markup(std::string(path.toUtf8().constData()));
    update();
}

void MarkupWidget::slot_savetracks(QString path) {
    markup.save_markup(std::string(path.toUtf8().constData()));
}

void MarkupWidget::slot_run() {
    qDebug() << "Rewrite using new MarkUp interface";
    markup.run();
    update();
    qDebug() << "run finished...";
}

// ни в коем случае не ставить тут обновление!!!
// произойдёт зацикливание сигналов...
void MarkupWidget::slot_framechanged(FrameWithControl *fwc) {
    int frameidx = fwc->getFrameIdx();
    QMap<int, ScaledBBox> bboxes;
    std::vector<DetectionAndTrack> detections;

    if (markup.get_frame(frameidx, &detections)) {
        for (auto det : detections) {
            ScaledBBox bbox = ScaledBBox(det);
            QVector<QPoint> track_line;
            for (auto point : det.track) {
                track_line.push_back(QPoint(point.x, point.y));
            }
            // qDebug() << track_line;
            bbox.setTrackLine(track_line, det.get_first_frame_idx());
            bboxes[det.track_id] = bbox;
        }
    }

    fwc->setMarkup(bboxes);
}

void MarkupWidget::slot_add_bbox(int frameidx, QRect bbox) {
    markup.add_detection(frameidx, bbox.topLeft().x(), bbox.topLeft().y(),
                         bbox.bottomRight().x(), bbox.bottomRight().y());
    update();
}

void MarkupWidget::slot_delete_bbox(int track_id, int frameidx) {
    markup.delete_detection(track_id, frameidx);
    update();
}

void MarkupWidget::slot_delete_track(int track_id) {
    markup.delete_track(track_id);
    update();
}

void MarkupWidget::slot_split_track(int track_id, int frameidx) {
    markup.split_track(track_id, frameidx);
    update();
}

void MarkupWidget::slot_unite_tracks() {
    markup.unite_tracks(fwcup->getTrackOnFocus(), fwcdn->getTrackOnFocus());
    update();
}

void MarkupWidget::update() {
    fwcup->update();
    fwcdn->update();
}
