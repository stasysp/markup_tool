#ifndef SCALEDBOX_H
#define SCALEDBOX_H

#include <QWidget>
#include <QRect>

#include "markup_backend/definitions.h"

class ScaledBBox
{
public:
    explicit ScaledBBox(int pxmin = 0, int pymin = 0, int pxmax = 1920, int pymax = 1080);
    explicit ScaledBBox(QPoint topleft, QPoint bottomright);
    explicit ScaledBBox(const Detection& det, int width=1920, int height=1080);

    QRect getScaledRect() const;
    // QRect getScaledRect(const QWidget* w) const;

    bool isInside(int x, int y) const;

private:
    int pxmin, pymin, pxmax, pymax;
};

/*class ScaledBBox
{
public:
    explicit ScaledBBox(float pxmin = 0., float pymin = 0., float pxmax = 1., float pymax = 1);
    explicit ScaledBBox(const Detection& det);

    QRect getScaledRect(int width, int height) const;
    QRect getScaledRect(const QWidget* w) const;

    bool isInside(float x, float y) const;

private:
    float pxmin, pymin, pxmax, pymax;
};*/

#endif // SCALEDBOX_H
