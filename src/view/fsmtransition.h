#ifndef FSMTRANSITION_H
#define FSMTRANSITION_H

#include <QWidget>

class FSMTransition : public QWidget
{
    Q_OBJECT
public:
    explicit FSMTransition(QWidget *parent = nullptr);
    /**
     * @brief relocateTransition
     * @param startPoint
     * @param startSize
     * @param finPoint
     * @param finSize
     */
    void relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize);

    void setSrc(QString nsrc);
    void setDst(QString ndst);
protected:
    /**
     * @brief paints the transition
     */
    void paintEvent(QPaintEvent *) override;



signals:

private:
    QPoint startPos;
    QPoint finPos;

    QString src;
    QString dst;

};

#endif // FSMTRANSITION_H
