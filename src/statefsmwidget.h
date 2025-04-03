#ifndef STATEFSMWIDGET_H
#define STATEFSMWIDGET_H

#include <QWidget>

namespace Ui {
class StateFSMWidget;
}

class StateFSMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StateFSMWidget(QWidget *parent = nullptr);
    ~StateFSMWidget();
    void mousePressEvent(QMouseEvent *event)override;
public slots:
    void changeStateName();
signals:
    void rightClick();
private:
    Ui::StateFSMWidget *ui;

};

#endif // STATEFSMWIDGET_H
