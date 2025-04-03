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
signals:
    void rightClick(QPoint position);
    void leftClick(QPoint position);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // WORKAREA_H
