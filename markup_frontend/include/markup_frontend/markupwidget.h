#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>

#include "maincontrolpanel.h"
#include "framewithcontrol.h"

#include "markup_backend/markup.h"
#include "markup_backend/tracks.h"

class MarkupWidget : public QWidget
{
    Q_OBJECT

public:
    MarkupWidget(QWidget *parent = nullptr);

public slots:
    void slot_set_video_path(QDir path);
    void slot_framechanged(FrameWithControl *fwc);
    void slot_run();

private:
    MainControlPanel *maincontrol = nullptr;

    FrameWithControl *fwcup = nullptr;
    FrameWithControl *fwcdn = nullptr;

    // проверить возможность удаления...
    // QDir path;
    MarkUp markup;
};

#endif // WIDGET_H
