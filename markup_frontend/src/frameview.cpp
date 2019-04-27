#include "markup_frontend/frameview.h"
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

#include "markup_frontend/scaledbox.h"

FrameView::FrameView(QWidget *parent) :
        QGraphicsView (parent) {}

FrameView::~FrameView() {}

void FrameView::mousePressEvent(QMouseEvent *event) {}
void FrameView::mouseReleaseEvent(QMouseEvent *event) {}
void FrameView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (scene != nullptr) {

        qDebug() << "double click event..." << event->localPos();
        qDebug() << scene->itemsBoundingRect().size() << this->size();
    }
}

void FrameView::keyPressEvent(QKeyEvent *event) {}

void FrameView::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
    qDebug() << "paint event #" << counter++;
}

void FrameView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    set_scene();
    // qDebug() << "resize event...";
}

void FrameView::set_scene() {
    // возможно, что эта логика избыточна и её можно заменить более простой
    if (!image.isNull()) {
        if (scene != nullptr) {
            delete scene;
        }
        scene = new QGraphicsScene(this); // object defined in header
        scene->addPixmap(image);

        for (auto iter = markup.begin(); iter != markup.end(); ++iter) {
            int key = iter.key();
            QColor color = QColor::fromHsv((67 * key) % 360, 255, 255, 127);
            QPen pen(color, 10, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
            QBrush brush(color);
            if (key == trackOnFocus) {
                scene->addRect(iter->getScaledRect(scene->width(), scene->height()), pen, brush);
            } else {
                scene->addRect(iter->getScaledRect(scene->width(), scene->height()), pen);
            }
        }

        this->setScene(scene);
        this->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    }
}

void FrameView::loadimagebypath(QString path) {
    image = QPixmap(path);
    QPaintEvent *event = new QPaintEvent(QRect());
    paintEvent(event);
}

void FrameView::slot_set_markup(QMap<int, ScaledBBox> newmarkup) {
    markup.clear();
    QMap<int, ScaledBBox>::const_iterator iter = newmarkup.constBegin();
    while (iter != newmarkup.constEnd()) {
        markup.add(iter.key(), iter.value());
        ++iter;
    }
    set_scene();
}
