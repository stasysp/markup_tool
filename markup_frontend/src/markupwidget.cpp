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
}
