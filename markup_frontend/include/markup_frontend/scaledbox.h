#ifndef SCALEDBOX_H
#define SCALEDBOX_H

#include <QWidget>
#include <QRect>

class ScaledBBox
{
public:
    explicit ScaledBBox(float pxmin = 0., float pymin = 0., float pxmax = 1., float pymax = 1);

    QRect getScaledRect(int width, int height) const;
    QRect getScaledRect(const QWidget* w) const;

private:
    float pxmin, pymin, pxmax, pymax;
};

#endif // SCALEDBOX_H
