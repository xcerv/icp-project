/**
 * Project name: ICP Project 2024/2025
 *
 * @file workarea.cpp
 * @author  xkadlet00
 *
 * @brief defines the structure of work area
 *
 */

#include "view/work_area/workarea.h"
#include <QPainter>

WorkArea::WorkArea() {
    size.setX(0);
    size.setY(0);
    setFixedSize(0,0);
}

WorkArea::~WorkArea(){}

void WorkArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClick(event->pos());
    } else if (event->button() == Qt::RightButton) {
        emit rightClick(event->pos());
    }
}

void WorkArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);  // force background color
    QWidget::paintEvent(event);  // call base class implementation
}

QPoint WorkArea::getSizeWA(){
    return size;
}

void WorkArea::setSizeWA(int x, int y){
    size.setX(x);
    size.setY(y);
    setFixedSize(x,y);
}

