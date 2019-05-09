#include "markup_frontend/scaledbox.h"
#include <iostream>

ScaledBBox::ScaledBBox(int left, int top, int right, int bottom, int width, int height) :
    _width(width), _height(height) {
    bbox = QRect(left, top, right + 1 - left, bottom + 1 - top);
    _checkSize();
}

ScaledBBox::ScaledBBox(QPoint topleft, QPoint bottomright, int width, int height) :
    _width(width), _height(height) {
    bbox = QRect(topleft, bottomright);
    _checkSize();
}

ScaledBBox::ScaledBBox(const Detection& det, int width, int height) :
    _width(width), _height(height) {
    QPoint topleft(det.bbox.x, det.bbox.y);
    QPoint bottomright(det.bbox.x + det.bbox.width, det.bbox.y + det.bbox.height);
    bbox = QRect(topleft, bottomright);
    _checkSize();
}

// проверить...
void ScaledBBox::_checkSize() {
    if (bbox.left() < 0) bbox.setLeft(0);
    if (bbox.right() >= _width) bbox.setRight(_width - 1);
    if (bbox.top() < 0) bbox.setTop(0);
    if (bbox.bottom() >= _height) bbox.setBottom(_height - 1);
}

QRect ScaledBBox::getScaledRect() const { return bbox; }

// и вот тут проверить логику...
// а лучше всего вообще переделать к херам...
bool ScaledBBox::isInside(int x, int y) const {
    if (x < bbox.left()) return false;
    if (x > bbox.right()) return false;
    if (y < bbox.top()) return false;
    if (y > bbox.bottom()) return false;
    return true;
}
