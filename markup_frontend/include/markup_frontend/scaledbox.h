#ifndef SCALEDBOX_H
#define SCALEDBOX_H

#include <QWidget>
#include <QRect>

#include "markup_backend/definitions.h"

class ScaledBBox
{
public:
    explicit ScaledBBox(float pxmin = 0., float pymin = 0., float pxmax = 1., float pymax = 1);
    explicit ScaledBBox(const Detection& det);

    QRect getScaledRect(int width, int height) const;
    QRect getScaledRect(const QWidget* w) const;

    bool isInside(float x, float y) const;

private:
    float pxmin, pymin, pxmax, pymax;
};

#endif // SCALEDBOX_H
