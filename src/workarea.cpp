#include "workarea.h"
#include <QPainter>

WorkArea::WorkArea() {
}

WorkArea::~WorkArea(){}

void WorkArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClick();
    } else if (event->button() == Qt::RightButton) {
        emit rightClick();
    }
}

void WorkArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);  // force background color
    QWidget::paintEvent(event);  // call base class implementation
}
