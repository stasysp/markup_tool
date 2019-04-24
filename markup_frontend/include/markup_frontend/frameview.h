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

    void setScaledPixmap(const QPixmap &pixmap);

private:
    void loadimagebypath(QString path);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap image;
    FrameMarkup markup;
    QGraphicsScene *scene = nullptr;
};

#endif // FRAMEVIEW_H
