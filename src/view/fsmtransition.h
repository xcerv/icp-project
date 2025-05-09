/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsmtransition.h
 * @author  xkadlet00
 * @author  xcervia00
 *
 * @brief Transition definition
 *
 */

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

    virtual ~FSMTransition();

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

    QSet<size_t> individualTransitions;

    QPushButton * editTransitionBtn = nullptr;
    QPoint btnSize;

    bool isLoop = false;

};

#endif // FSMTRANSITION_H
