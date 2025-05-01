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
    /**
     * @brief mousePressEvent to know when user is clicking into work area
     * @param event user input from mouse
     */
    void mousePressEvent(QMouseEvent *event)override;
    /**
     * @brief sets size of work area
     * @param x coordinate
     * @param y coordinate
     */
    void setSizeWA(int x, int y);
    /**
     * @brief gets the current size of work area
     * @return size of work area
     */
    QPoint getSizeWA();
signals:
    /**
     * @brief signal to be sent when rigt mouse click happens
     * @param position of right click within work area
     */
    void rightClick(QPoint position);
    /**
     * @brief signal to be sent when left mouse click happens
     * @param position of left click within work area
     */
    void leftClick(QPoint position);
protected:
    /**
     * @brief paints work area white
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint size;//current size of work area
};

#endif // WORKAREA_H
