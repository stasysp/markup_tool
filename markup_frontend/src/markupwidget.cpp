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
