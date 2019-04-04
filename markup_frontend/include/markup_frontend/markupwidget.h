#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

#include "maincontrolpanel.h"
#include "frameview.h"
#include "framecontrol.h"

class MarkupWidget : public QWidget
{
    // Q_OBJECT

public:
    MarkupWidget(QWidget *parent = nullptr);

private slots:
    // void load_file();

private:
    MainControlPanel *maincontrol = nullptr;
    FrameView *frameviewup = nullptr;
    FrameView *frameviewdown = nullptr;
    FrameControl *framecontrolup = nullptr;
    FrameControl *framecontroldown = nullptr;
};

#endif // WIDGET_H
