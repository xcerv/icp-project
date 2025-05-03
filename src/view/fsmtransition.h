#ifndef FSMTRANSITION_H
#define FSMTRANSITION_H

#include <QPushButton>
#include <QWidget>
#include <QSet>

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

    ~FSMTransition();

    void setSrc(QString nsrc);
    void setDst(QString ndst);

    void addTransition(size_t num);
    void subTransition(size_t num);
    QSet<size_t> getTransitions();
signals:
    void editTransition(FSMTransition *);
protected:
    /**
     * @brief paints the transition
     */
    void paintEvent(QPaintEvent *) override;
    /**
     * @brief clicked on the edit button
     */
    void clickedEdit();

private:
    QPoint startPos;
    QPoint finPos;

    QString src;
    QString dst;

    QSet<size_t> individualTransitions;

    QPushButton * editTransitionBtn = nullptr;
    QPoint btnSize;

};

#endif // FSMTRANSITION_H
