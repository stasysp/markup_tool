#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include "markup_frontend/maincontrolpanel.h"

MainControlPanel::MainControlPanel(QWidget *parent) : QWidget(parent)
{
    selectmodel = new QComboBox();
    //selectmodel->addItem('SSD');
    //selectmodel->addItem('DarkNet');

    loadmodel = new QPushButton("Load Model");
    loadvideo = new QPushButton("Load Video");
    loadtracks = new QPushButton("Load Tracks");
    run = new QPushButton("Run Object Tracking");
    play = new QPushButton("Play Video");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(selectmodel);
    layout->addWidget(loadmodel);
    layout->addWidget(loadvideo);
    layout->addWidget(loadtracks);
    layout->addWidget(run);
    layout->addWidget(play);

    connect(loadvideo, &QPushButton::clicked, this, &MainControlPanel::slot_loadvideo);
    connect(loadmodel, &QPushButton::clicked, this, &MainControlPanel::slot_loadmodel);
    connect(loadtracks, &QPushButton::clicked, this, &MainControlPanel::slot_loadtracks);
    connect(run, &QPushButton::clicked, this, &MainControlPanel::slot_run);
    connect(play, &QPushButton::clicked, this, &MainControlPanel::slot_play);
}

void MainControlPanel::slot_loadvideo() {
    auto path = QFileDialog::getExistingDirectory(this, "Open Folder with frames");
    QDir dir(path);
    emit send_path(dir);
    // call some backend-function
}

void MainControlPanel::slot_loadmodel() {
    auto path = QFileDialog::getOpenFileName(this, "Load Model", "",
                                             "*.model");
    qDebug() << "model path : " << path;
    // call some backend-function
}

void MainControlPanel::slot_loadtracks() {
    auto path = QFileDialog::getOpenFileName(this, "Open Tracks", "",
                                             "*.tracks");
    qDebug() << "tracks path : " << path;
    // call some backend-function
}

void MainControlPanel::slot_run() {
    qDebug() << "run tracking...";
    // call some backend-function (run)
}

void MainControlPanel::slot_play() {
    qDebug() << "play video...";
    // блокировать панели редактирования
    // запустить воспроизведение кадров (т.е. инкрементить кадры)
    // ждать сигнала стоп.
}
