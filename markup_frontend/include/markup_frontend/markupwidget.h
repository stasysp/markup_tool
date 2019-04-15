#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
// #include <QPushButton>
#include <QDir>

#include "maincontrolpanel.h"
#include "framewithcontrol.h"


class MarkupWidget : public QWidget
{
    Q_OBJECT

public:
    MarkupWidget(QWidget *parent = nullptr);

private:
    MainControlPanel *maincontrol = nullptr;

    FrameWithControl *fwcup = nullptr;
    FrameWithControl *fwcdn = nullptr;
};

#endif // WIDGET_H
