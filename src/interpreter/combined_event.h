/**
* Project name: ICP Project 2024/2025
*
* @file combined_event.h
* @author  xcervia00
*
* @brief Combined event (inputEvent + async Timeout) used in the FSM application - interface
*
*/

#ifndef COMBINED_EVENT_H
#define COMBINED_EVENT_H

#include <QObject>
#include <QEvent>
#include <QPointer>

#include "combined_transition.h"

/**
 * @brief Event serving as unified interface for inputs coming to interpreted Fsm
 */
class FsmInputEvent : public QEvent
{
    private:
        QString name; ///< Name of the Input that triggered this event
        
    public:
        static const QEvent::Type m_eventType ; ///< Static Custom EventId

        /**
         * @brief Constructor for Input event
         * @param inName The name of the Input that triggered this event
         */
        FsmInputEvent(const QString &inName);

        /**
         * @brief Getter for the name of this event (used to trigger transitions with matching input)
         * @return Returns the reference to name of this event
         */
        const QString &getName() const;

        /**
         * @brief Getter for the unique identifier of this event
         * @return Returns the id of this event
         */
        static QEvent::Type getType();
};


/**
 * @brief Event spawned by transition if a timeout is specified
 */
class FsmTimeoutEvent : public QEvent
{
    private:
        QPointer<QAbstractTransition> m_identity; ///< Identity of the transition that sent this event

    public:
        const static QEvent::Type m_eventType ; ///< Static Custom EventId

        /**
         * @brief Constructor for Timeout event
         * @param objIdentity The identity of the object that spawned this event
         */
        FsmTimeoutEvent(QAbstractTransition* objIdentity);

        /**
         * @brief Returns the identity of the transition that spawned this event
         * @return Returns the pointer to the CombinedTransition
         */
        QAbstractTransition* getIdentity();

        /**
         * @brief Getter for the unique identifier of this event
         * @return Returns the id of this event
         */
        static QEvent::Type getType();
};

#endif // COMBINED_EVENT_H
