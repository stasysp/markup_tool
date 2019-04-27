#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QString>
#include "framemarkup.h"

class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FrameView(QWidget* parent = nullptr);
    ~FrameView();

    void loadimagebypath(QString path);

public slots:
    void slot_set_markup(QMap<int, ScaledBBox> newmarkup);

private:
    void set_scene();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int counter = 0;
    int trackOnFocus = 4;
    QPixmap image;
    FrameMarkup markup;
    QGraphicsScene *scene = nullptr;
};

#endif // FRAMEVIEW_H
