#include "markup_frontend/markupwidget.h"
#include "markup_frontend/frameview.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

MarkupWidget::MarkupWidget(QWidget *parent)
        : QWidget(parent)
{
    maincontrol = new MainControlPanel(this);
    frameviewup = new FrameView(this);
    frameviewdown = new FrameView(this);
    framecontrolup = new FrameControl(this);
    framecontroldown = new FrameControl(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QVBoxLayout* framelayout = new QVBoxLayout;
    QVBoxLayout* fcontlayout = new QVBoxLayout;

    hlayout->addWidget(maincontrol);
    hlayout->addLayout(framelayout);
    hlayout->addLayout(fcontlayout);

    framelayout->addWidget(frameviewup);
    framelayout->addWidget(frameviewdown);
    fcontlayout->addWidget(framecontrolup);
    fcontlayout->addWidget(framecontroldown);

    connect(maincontrol, &MainControlPanel::send_path, this, &MarkupWidget::slot_set_path);
    connect(this, &MarkupWidget::send_nframes, framecontrolup, &FrameControl::slot_set_nframes);
    connect(this, &MarkupWidget::send_nframes, framecontroldown, &FrameControl::slot_set_nframes);
}

void MarkupWidget::slot_set_path(QDir dir) {
    if (imagiesdir != dir) {
        qDebug() << "dir set as : " << imagiesdir;
        imagiesdir = dir;

        emit send_reset();
    }

    imagiesdir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);   //устанавливаем фильтр выводимых файлов/папок (см ниже)
    // dir.setSorting(QDir::Size | QDir::Reversed);   //устанавливаем сортировку "от меньшего к большему"
    QFileInfoList list = imagiesdir.entryInfoList();     //получаем список файлов директории
    emit send_nframes(list.size());
    // for (int i = 0; i < list.size(); ++i) {
    //     QFileInfo fileInfo = list.at(i);
    //     qDebug() << fileInfo.fileName();
    // }
}