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

    signals:
            void send_nframes(int nframes);
    void send_reset();

private slots:
            void slot_set_path(QDir dir);

private:
    MainControlPanel *maincontrol = nullptr;
    // FrameView *frameviewup = nullptr;
    // FrameView *frameviewdown = nullptr;
    // FrameControl *framecontrolup = nullptr;
    // FrameControl *framecontroldown = nullptr;

    FrameWithControl *fwcup = nullptr;
    FrameWithControl *fwcdn = nullptr;

    // добавить проверку существования???
    QDir imagiesdir;
};


#endif // WIDGET_H
