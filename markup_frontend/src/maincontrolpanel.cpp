#include <QGridLayout>

#include "markup_frontend/maincontrolpanel.h"

MainControlPanel::MainControlPanel(QWidget *parent) : QWidget(parent)
{
    loadmodel = new QPushButton("Load Model");
    loadvideo = new QPushButton("Load Video");
    loadtracks = new QPushButton("Load Tracks");
    run = new QPushButton("Run Object Tracking");
    play = new QPushButton("Play Video");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(loadmodel);
    layout->addWidget(loadvideo);
    layout->addWidget(loadtracks);
    layout->addWidget(run);
    layout->addWidget(play);

    // connect(loadimage, &QPushButton::clicked, this, &MarkupWidget::load_file);
}
