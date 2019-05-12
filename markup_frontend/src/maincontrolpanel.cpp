#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QLabel>

#include "markup_frontend/maincontrolpanel.h"
#include "markup_backend/markup.h"

MainControlPanel::MainControlPanel(QWidget *parent) : QWidget(parent)
{
    //selectmodel = new QComboBox();
    //selectmodel->addItem("SSD");
    //selectmodel->addItem("DarkNet");

    loadvideo = new QPushButton("Load Video");
    loadtracks = new QPushButton("Load Tracks");
    savetracks = new QPushButton("Save Tracks");
    run = new QPushButton("Run Object Tracking");
    play = new QPushButton("Play Video");
    step_forward = new QPushButton("Step Forward");
    step_backward = new QPushButton("Step Backward");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *lbl = new QLabel();
    lbl->setSizePolicy(QSizePolicy());
    lbl->setText("Model selection:");

    int BIGSPACE = 2;
    int SMALLSPACE = 1;

    layout->addStretch(4 * SMALLSPACE);
    //layout->addWidget(lbl);
    //layout->addWidget(selectmodel);
    //layout->addStretch(BIGSPACE);
    layout->addWidget(loadvideo);
    layout->addStretch(SMALLSPACE);
    layout->addWidget(loadtracks);
    layout->addStretch(SMALLSPACE);
    layout->addWidget(savetracks);
    layout->addStretch(SMALLSPACE);
    layout->addWidget(run);
    layout->addStretch(BIGSPACE);
    layout->addWidget(step_forward);
    layout->addStretch(SMALLSPACE);
    layout->addWidget(step_backward);
    layout->addStretch(SMALLSPACE);
    layout->addWidget(play);
    layout->addStretch(SMALLSPACE);

    connect(loadvideo, &QPushButton::clicked, this, &MainControlPanel::slot_loadvideo);
    connect(loadtracks, &QPushButton::clicked, this, &MainControlPanel::slot_loadtracks);
    connect(savetracks, &QPushButton::clicked, this, &MainControlPanel::slot_savetracks);
    connect(run, &QPushButton::clicked, this, &MainControlPanel::slot_run);
    connect(play, &QPushButton::clicked, this, &MainControlPanel::slot_play);
    connect(step_forward, &QPushButton::clicked, this, &MainControlPanel::slot_step_forward);
    connect(step_backward, &QPushButton::clicked, this, &MainControlPanel::slot_step_backward);
}

void MainControlPanel::slot_loadvideo() {
    auto path = QFileDialog::getExistingDirectory(this, "Open Folder with frames");
    QDir dir(path);
    emit send_video_path(dir);
    // call some backend-function
}

void MainControlPanel::slot_loadtracks() {
    auto path = QFileDialog::getOpenFileName(this, "Open Tracks", "",
                                             "*.txt");
    emit send_tracks_path(path);
    qDebug() << "tracks path : " << path;
    // call some backend-function
}

void MainControlPanel::slot_savetracks() {
    auto path = QFileDialog::getSaveFileName(this, "Open Tracks", "",
                                             "*.txt");
    emit send_savetracks(path);
    qDebug() << "tracks path : " << path;
    // call some backend-function
}

void MainControlPanel::slot_run() {
    qDebug() << "run tracking...";
    emit send_run();
}

void MainControlPanel::slot_play() {
    qDebug() << "play video...";
    // блокировать панели редактирования
    // запустить воспроизведение кадров (т.е. инкрементить кадры)
    // ждать сигнала стоп.
}

void MainControlPanel::slot_step_forward() {
    emit send_move_all_frames(1);
}

void MainControlPanel::slot_step_backward() {
    emit send_move_all_frames(-1);
}
