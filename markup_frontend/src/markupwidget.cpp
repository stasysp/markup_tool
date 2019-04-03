#include "markup_frontend/markupwidget.h"
#include "markup_frontend/frameview.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

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

    // connect(loadimage, &QPushButton::clicked, this, &MarkupWidget::load_file);
}

/*void MarkupWidget::load_file() {
    auto path = QFileDialog::getOpenFileName(this, "Open Dialog", "",
                                             "*.png *.jpg *.bmp *.JPG");
    if (path.isEmpty()) return;
    QPixmap image(path);
    frameviewup->setScaledPixmap(image);
}*/
