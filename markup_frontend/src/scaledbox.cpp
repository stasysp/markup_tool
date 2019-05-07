#include "markup_frontend/scaledbox.h"
#include <iostream>

ScaledBBox::ScaledBBox(int pxmin, int pymin, int pxmax, int pymax) :
    pxmin(pxmin), pymin(pymin), pxmax(pxmax), pymax(pymax) {}

ScaledBBox::ScaledBBox(QPoint topleft, QPoint bottomright) :
    pxmin(topleft.x()), pymin(topleft.y()), pxmax(bottomright.x()), pymax(bottomright.y()) {}

ScaledBBox::ScaledBBox(const Detection& det, int width, int height) {
    pxmin = det.bbox.x;
    pymin = det.bbox.y;
    pxmax = det.bbox.x + det.bbox.width;
    pymax = det.bbox.y + det.bbox.height;

    if (pxmin < 0) {
        std::cout << "pxmin < 0";
        pxmin = 0;
    }

    if (pymin < 0) {
        std::cout << "pymin < 0";
        pymin = 0;
    }

    // проверить эту логику!!!
    if (width <= pxmax) {
        std::cout << "pxmax > 1";
        pxmax = width - 1;
    }

    if (height <= pymax) {
        std::cout << "pymax > 1";
        pymax = height - 1;
    }
}

QRect ScaledBBox::getScaledRect() const {
    QPoint topleft = QPoint(pxmin, pymin);
    QPoint bottomright = QPoint(pxmax, pymax);

    return QRect(topleft, bottomright);
}

// и вот тут проверить логику... а лучше всего вообще переделать к херам...
bool ScaledBBox::isInside(int x, int y) const {
    return (x >= pxmin) && (x <= pxmax) && (y >= pymin) && (y <= pymax);
}
