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

QPoint FrameView::getImagePoint(QMouseEvent *event) {
    if (scene != nullptr) {
        int mX = event->localPos().x();
        int mY = event->localPos().y();
        int wW = this->size().width();
        int wH = this->size().height();
        int sW = sceneRect().width();
        int sH = sceneRect().height();

        double scale = std::max((double) sH / (double) wH, (double) sW / (double) wW);

        // подумать, проверить округления...
        // детекции получаются точными. но вопрос с рамками. нужно проверить
        int pX = sW / 2 + (int) (scale * (mX - wW / 2));
        int pY = sH / 2 + (int) (scale * (mY - wH / 2));

        return QPoint(pX, pY);
    } else {
        return QPoint();
    }
}

void FrameView::mousePressEvent(QMouseEvent *event) {
    if (!isSelectMode) {
        mousePressed = true;
        QPoint point = getImagePoint(event);
        tempRect.setTopLeft(point);
        tempRect.setBottomRight(point);
    }
}

void FrameView::mouseReleaseEvent(QMouseEvent *event) {
    if (mousePressed) {
        mousePressed = false;
        tempRect.setBottomRight(getImagePoint(event));
        send_add_bbox(tempRect);
    }
}

void FrameView::mouseDoubleClickEvent(QMouseEvent *event) {
    // нужна ли эта проверка?
    if (isSelectMode && (scene != nullptr)) {
        QPoint point = getImagePoint(event);

        int get_track = markup.select_bbox(point.x(), point.y());
        if (get_track >= 0) {
            trackOnFocus = get_track;
        }
        set_scene();
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
}

void FrameView::set_scene() {
    qDebug() << "set scene...";
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
                scene->addRect(iter->getScaledRect(), pen, brush);
            } else {
                scene->addRect(iter->getScaledRect(), pen);
            }
        }

        this->setScene(scene);
        this->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    }
}

void FrameView::loadimagebypath(QString path) {
    image = QPixmap(path);
    //QPaintEvent *event = new QPaintEvent(QRect());
    //paintEvent(event);
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

int FrameView::getTrackOnFocus() {
    return trackOnFocus;
}

void FrameView::update() {
    set_scene();
    qDebug() << "updete in frameview...";
    QPaintEvent *event = new QPaintEvent(QRect(0,0,1920,1080));
    paintEvent(event);
}
