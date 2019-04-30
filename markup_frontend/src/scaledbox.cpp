#include "markup_frontend/scaledbox.h"
#include <iostream>

ScaledBBox::ScaledBBox(float pxmin, float pymin, float pxmax, float pymax) :
    pxmin(pxmin), pymin(pymin), pxmax(pxmax), pymax(pymax) {}

ScaledBBox::ScaledBBox(const Detection& det) {
    float img_height = 1080.;
    float img_width = 1920.;
    pxmin = float(det.bbox.x) / img_width;
    pymin = float(det.bbox.y) / img_height;
    pxmax = float(det.bbox.x + det.bbox.width) / img_width;
    pymax = float(det.bbox.y + det.bbox.height) / img_height;

    if (pxmin < 0) {
        std::cout << "pxmin < 0";
        pxmin = 0;
    }

    if (pymin < 0) {
        std::cout << "pymin < 0";
        pymin = 0;
    }

    if (pxmax > 1) {
        std::cout << "pxmax > 1";
        pxmax = 1;
    }

    if (pymax > 1) {
        std::cout << "pymax > 1";
        pymax = 1;
    }
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

bool ScaledBBox::isInside(float x, float y) const {
    return (x >= pxmin) && (x <= pxmax) && (y >= pymin) && (y <= pymax);
}
