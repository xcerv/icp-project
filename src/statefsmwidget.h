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

class StateFSMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StateFSMWidget(QPoint pos, QWidget *parent = nullptr);
    ~StateFSMWidget();
    void mousePressEvent(QMouseEvent *event)override;
    void setName(QString name);

    QPoint getSize();
    QPoint getPosition();
public slots:
    void addCondition(QString cond);
signals:
    void rightClick();
private:
    Ui::StateFSMWidget *ui;
    QLayout * scrollLayout;
    QWidget * scrollContainer;
    std::vector<QLabel*> allConditions;
    QPoint position;
    QPoint size;
};

#endif // STATEFSMWIDGET_H
