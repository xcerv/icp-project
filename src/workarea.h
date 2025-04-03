/**
 * Project name: ICP Project 2024/2025
 *
 * @file workarea.h
 * @author  xkadlet00
 *
 * @brief defines the structure of work area
 *
 */

#ifndef WORKAREA_H
#define WORKAREA_H

#include <QWidget>
#include <QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class WorkArea;
}
QT_END_NAMESPACE

class WorkArea : public QWidget
{
    Q_OBJECT
public:
    WorkArea();
    ~WorkArea();
    void mousePressEvent(QMouseEvent *event)override;
    void setSizeWA(int x, int y);
    QPoint getSizeWA();
signals:
    void rightClick(QPoint position);
    void leftClick(QPoint position);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPoint size;
};

#endif // WORKAREA_H
