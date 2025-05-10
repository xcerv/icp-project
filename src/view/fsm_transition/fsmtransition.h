/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsmtransition.h
 * @author  xkadlet00
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
     * @brief draws the transition based on the specified states its ment to connect
     * @param startPoint coordinate of top-left corner of 1. state
     * @param startSize size of 1. state
     * @param finPoint coordinate of top-left corner of 1. state
     * @param finSize size of 2. state
     */
    void relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize);

    virtual ~FSMTransition();

    /**
     * @brief adds a number of a transition this UI element is ment to represent
     * @param num ID of transition
     */
    void addTransition(size_t num);
    void subTransition(size_t num);
    /**
     * @brief removes a number of a transition this UI element is ment to represent
     * @param num ID of transition
     */
    QSet<size_t> getTransitions();
signals:
    /**
     * @brief signal is emitted when the edit button is clicked
     */
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
    QPoint startPos;///< start position of UI transition
    QPoint finPos;///< end position of UI transition

    QSet<size_t> individualTransitions;///< all the IDs of transitions this UI emement represents

    QPushButton * editTransitionBtn = nullptr;///< push button to signify the user wants to edit transitions
    QPoint btnSize;///< size of the push button

    bool isLoop = false;///< true if the two connected states are one and the same

};

#endif // FSMTRANSITION_H
