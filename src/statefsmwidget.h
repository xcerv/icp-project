/**
 * Project name: ICP Project 2024/2025
 *
 * @file statefsmwidget.h
 * @author  xkadlet00
 *
 * @brief defines the structure of a FSM state
 *
 */

#ifndef STATEFSMWIDGET_H
#define STATEFSMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <vector>

namespace Ui {
class StateFSMWidget;
}

/**
 * @brief Widget for displaying a FSM state
 */
class StateFSMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StateFSMWidget(QPoint pos, QWidget *parent = nullptr);
    ~StateFSMWidget();
    /**
     * @brief adds mousePressEvent
     * @param event what happend (mouse left click / mouse right click)
     */
    void mousePressEvent(QMouseEvent *event)override;
    /**
     * @brief sets name of state
     * @param name name of state to be displayed
     */
    void setName(QString name);
    /**
     * @brief gets name of state
     * @return name of state
     */
    QString getName();
    /**
     * @brief returns size of state
     * @return size
     */
    QPoint getSize();
    /**
     * @brief returns position of state within workArea
     * @return position
     */
    QPoint getPosition();
    /**
     * @brief sets new position of state
     * @param pos new position (move there)
     */
    void setPosition(QPoint positon);
    /**
     * @brief set the output of the state
     * @param cond output to be displayed
     */
    void setOutput(QString cond);
    /**
     * @brief get the output of the state
     * @return output of state
     */
    QString getOutput();
    /**
     * @brief recolors the state
     * @param c1 primary color (of body)
     * @param c2 secondary color (of text)
     */
    void recolor(const QString& c1, const QString& c2);
signals:
    /**
     * @brief rightClick onto state
     */
    void rightClick();
    /**
     * @brief leftClick onto state
     */
    void leftClick();
private:
    Ui::StateFSMWidget *ui;
    QLayout * scrollLayout; //layout for scroll area
    QWidget * scrollContainer; //container for scroll area
    QLabel* output; //outputs of state
    QPoint position; //position of state wothin workArea
    QPoint size; //size of state
};

#endif // STATEFSMWIDGET_H
