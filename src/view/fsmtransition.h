#ifndef FSMTRANSITION_H
#define FSMTRANSITION_H

#include <QWidget>

class FSMTransition : public QWidget
{
    Q_OBJECT
public:
    explicit FSMTransition(QWidget *parent = nullptr);
protected:
    /**
     * @brief paints the transition
     */
    void paintEvent(QPaintEvent *) override;

    void relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize);

signals:

private:
    QPoint startPos;
    QPoint finPos;

};

#endif // FSMTRANSITION_H
