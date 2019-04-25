#include "markup_frontend/scaledbox.h"

ScaledBBox::ScaledBBox(float pxmin, float pymin, float pxmax, float pymax) :
    pxmin(pxmin), pymin(pymin), pxmax(pxmax), pymax(pymax) {}

ScaledBBox::ScaledBBox(const Detection& det) {
    float img_height = 480.;
    float img_width = 640.;
    pxmin = float(det.bbox.x) / img_width;
    pymin = float(det.bbox.y) / img_height;
    pxmax = float(det.bbox.x + det.bbox.width) / img_width;
    pymax = float(det.bbox.y + det.bbox.height) / img_height;
}

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
