#include "markup_frontend/frameview.h"
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QGraphicsScene>

#include "markup_frontend/scaledbox.h"

FrameView::FrameView(QWidget *parent) :
    QGraphicsView (parent)
{
    markup.add(5, ScaledBBox{0.1, 0.2, 0.5, 0.7});
    markup.add(13, ScaledBBox{0.8, 0.9, 0.2, 0.6});
    markup.add(2, ScaledBBox{0.5, 0.6, 0.7, 0.8});
    markup.add(10, ScaledBBox{0.4, 0.7, 0.3, 0.9});
}

FrameView::~FrameView() {}

void FrameView::setScaledPixmap(const QPixmap &pixmap) {
    image = pixmap;
    update();
}

void FrameView::mousePressEvent(QMouseEvent *event) {}

void FrameView::mouseReleaseEvent(QMouseEvent *event) {}

void FrameView::keyPressEvent(QKeyEvent *event) {}

void FrameView::paintEvent(QPaintEvent *event) {
    // возможно, что эта логика избыточна и её можно заменить более простой
    if (!image.isNull()) {
        scene = new QGraphicsScene(this); // object defined in header
        this->setScene(scene);
        scene->addPixmap(image);
        QPen pen(Qt::green, 10, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);

        for (auto iter = markup.begin(); iter != markup.end(); ++iter) {
            scene->addRect(iter->getScaledRect(scene->width(), scene->height()), pen);
        }

        this->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    }
    QGraphicsView::paintEvent(event);
}
