#include <QHBoxLayout>
#include <QDebug>

#include "markup_frontend/framewithcontrol.h"


FrameWithControl::FrameWithControl(QWidget *parent) : QWidget(parent)
{
    frameview = new FrameView(this);
    framecontrol = new FrameControl(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(frameview);
    hlayout->setStretch(0, 1);
    hlayout->addWidget(framecontrol);
    hlayout->setStretch(1, 0);

    connect(framecontrol, &FrameControl::send_newframeidx,
            this, &FrameWithControl::setFrameIdx);
    connect(framecontrol->btn_del_bbox, &QPushButton::clicked,
            this, &FrameWithControl::slot_delete_bbox);
    connect(framecontrol->btn_del_track, &QPushButton::clicked,
            this, &FrameWithControl::slot_delete_track);
    connect(framecontrol->btn_spl_track, &QPushButton::clicked,
            this, &FrameWithControl::slot_split_track);
    connect(framecontrol->btn_unt_track, &QPushButton::clicked,
            this, &FrameWithControl::slot_unite_tracks);
    connect(frameview, &FrameView::send_add_bbox,
            this, &FrameWithControl::slot_add_bbox);
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
    QFileInfoList img_list = img_path.entryInfoList();     //получаем список файлов директории
    QString file_name = img_list[frameidx].filePath();
    frameview->loadimagebypath(file_name);

    emit send_framechanged(this);
}

void FrameWithControl::setPath(QDir path) {
    if (img_path != path) {
        img_path = path;
        img_path.setFilter(QDir::Files | QDir::NoSymLinks);   //устанавливаем фильтр выводимых файлов/папок (см ниже)
        reset();
    }
}

void FrameWithControl::reset() {
    QFileInfoList img_list = img_path.entryInfoList();     //получаем список файлов директории
    n_frames = img_list.size();
    frameidx = 0;
    doOnFrameChange();
}

void FrameWithControl::setMarkup(QMap<int, ScaledBBox> markup) {
    frameview->slot_set_markup(markup);
}

void FrameWithControl::slot_add_bbox(QRect bbox) {
    emit send_add_bbox(frameidx, bbox);
}

void FrameWithControl::slot_delete_bbox() {
    emit send_delete_bbox(frameview->getTrackOnFocus(), frameidx);
}

void FrameWithControl::slot_delete_track() {
    emit send_delete_track(frameview->getTrackOnFocus());
}

void FrameWithControl::slot_split_track() {
    emit send_split_track(frameview->getTrackOnFocus(), frameidx);
}

void FrameWithControl::slot_unite_tracks() {
    emit send_unite_tracks();
}

void FrameWithControl::update() {
    emit send_framechanged(this);
    frameview->update();
    qDebug() << "frameview UPDATE...";
}
