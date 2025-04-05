/**
* Project name: ICP Project 2024/2025
*
* @file combined_event.h
* @author  xcervia00
*
* @brief Combined event (string + cond +timeout) used in the FSM application interface
*
*/

#ifndef COMBINED_EVENT_H
#define COMBINED_EVENT_H

#include <QObject>
#include <QEvent>
#include <QPointer>

#include "combined_transition.h"

/* Input Event */
class FsmInputEvent : public QEvent
{
    private:
        QString name;
        QString value;
        
    public:
        static inline QEvent::Type m_eventType ; ///< Static Custom EventId

        // Methods
        FsmInputEvent(const QString &inName, const QString &inVal);

        const QString &getName() const;
        static QEvent::Type getType();
};


/* Timeout Event */
class FsmTimeoutEvent : public QEvent
{
    private:
        QPointer<QAbstractTransition> m_identity; ///< Identity of the transition that sent this event

    public:
        static inline QEvent::Type m_eventType ; ///< Static Custom EventId

        // Methods

        FsmTimeoutEvent(QAbstractTransition* objIdentity);

        
        QAbstractTransition* getIdentity();
        static QEvent::Type getType();
};

#endif // COMBINED_EVENT_H
