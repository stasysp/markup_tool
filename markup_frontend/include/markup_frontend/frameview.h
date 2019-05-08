#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QString>
#include <QPoint>
#include "framemarkup.h"

class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FrameView(QWidget* parent = nullptr);
    ~FrameView();

    void loadimagebypath(QString path);
    void update();

signals:
    void send_add_bbox(QRect bbox);
    void send_mode(bool isSelectMode);

public slots:
    void slot_set_select_mode() {
        isSelectMode = true;
        emit send_mode(isSelectMode);
    }
    void slot_set_addbbox_mode() {
        isSelectMode = false;
        emit send_mode(isSelectMode);
    }
    void slot_set_markup(QMap<int, ScaledBBox> newmarkup);
    void set_scene();
    int getTrackOnFocus();
    QPoint getImagePoint(QMouseEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int counter = 0;
    int trackOnFocus = -1;
    QPixmap image;
    FrameMarkup markup;
    QGraphicsScene *scene = nullptr;

    // отрисовка временного прямоугольника...
    // QPainter painter;
    QRect tempRect;
    bool mousePressed = false;
    bool drawStarted = false;

    bool isSelectMode = true;
};

#endif // FRAMEVIEW_H
