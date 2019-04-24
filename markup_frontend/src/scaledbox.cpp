#include "markup_frontend/scaledbox.h"

ScaledBBox::ScaledBBox(float pxmin, float pymin, float pxmax, float pymax) :
    pxmin(pxmin), pymin(pymin), pxmax(pxmax), pymax(pymax) {}

QRect ScaledBBox::getScaledRect(int width, int height) const {
    QPoint topleft = QPoint(pxmin * width, pymin * height);
    QPoint bottomright = QPoint(pxmax * width, pymax * height);

    return QRect(topleft, bottomright);
}

QRect ScaledBBox::getScaledRect(const QWidget* w) const {
    int width = w->width();
    int height = w->height();
    QPoint topleft = QPoint(pxmin * width, pymin * height);
    QPoint bottomright = QPoint(pxmax * width, pymax * height);

    return QRect(topleft, bottomright);
}
