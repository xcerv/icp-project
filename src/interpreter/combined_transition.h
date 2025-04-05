/**
* Project name: ICP Project 2024/2025
*
* @file combined_transition.h
* @author  xcervia00
*
* @brief Combined transition (string + cond + timeout) used in the FSM application
*
*/

#ifndef COMBINED_TRANSITION_H
#define COMBINED_TRANSITION_H

#include <QObject>
#include <QStateMachine>
#include <QAbstractTransition>

class CombinedTransition : public QAbstractTransition
{
    Q_OBJECT

    public:
    CombinedTransition(const QString &name, const QString &guard, const QString &timeout);

    // Stop timer - might be performed for all transitions always onTransition?
    // Parsing might be perfomed by model
    // Empty string possible for name, guard and even timeout - there will always be a timer

    protected:
        bool eventTest(QEvent *e) override;


        void onTransition(QEvent *) override;
        
    private:
        QString m_name;
        QString m_guard;
        QString m_timeout;

        bool m_pending;
        int m_pending_id;

        QStateMachine *m_parentMachine;
};

#endif // COMBINEDTRANSITION_H
