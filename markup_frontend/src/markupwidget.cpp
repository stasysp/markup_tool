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

    connect(maincontrol, &MainControlPanel::send_path, this, &MarkupWidget::slot_set_path);
    // connect(this, &MarkupWidget::send_nframes, framecontrolup, &FrameControl::slot_set_nframes);
    // connect(this, &MarkupWidget::send_nframes, framecontroldown, &FrameControl::slot_set_nframes);
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