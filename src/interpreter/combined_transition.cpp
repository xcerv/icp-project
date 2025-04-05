/**
* Project name: ICP Project 2024/2025
*
* @file combined_transition.cpp
* @author  xcervia00
*
* @brief Combined transition (string + cond + timeout) used in the FSM application
*
*/

#include "combined_transition.h"
#include "combined_event.h"

#include <QObject>
#include <QStateMachine>
 
CombinedTransition::CombinedTransition(const QString &name, const QString &guard, const QString &timeout) 
    :
    m_name{name}, 
    m_guard{guard}, 
    m_timeout{timeout},
    m_pending{false},
    m_pending_id{-1},
    m_parentMachine{static_cast<QStateMachine*>(this->parent()->parent())}
{
}

bool CombinedTransition::eventTest(QEvent *e)
{
    if(e == nullptr) return false;


    if(e->type() == FsmInputEvent::getType()) // Initial input trigger
    {
        // Already waiting for timeout, wait for it instead
        if(m_pending)
            return false;

        // Mismatch in the input name
        if(static_cast<FsmInputEvent*>(e)->getName() != this->m_name)
            return false;

        // Try guard condition here...
        if(m_guard != "")
        {
            // QScriptEngine check here...
            // return false // Failed check ==> don't transition
        }

        // Guard passed... start timeout
        bool ok;
        int timeoutMs = this->m_timeout.toInt(&ok);
        if(!ok || timeoutMs < 0){timeoutMs = 0;}

        // Start new timeout
        this->m_pending_id = m_parentMachine->postDelayedEvent(new FsmTimeoutEvent(this), timeoutMs);
        this->m_pending = true;

    } else if(e->type() == FsmTimeoutEvent::getType())  // Something timed-out - was it me?
    {
        // Not even awaiting timeout, exit...
        if(!m_pending)
            return false;

        FsmTimeoutEvent *timeEvent = static_cast<FsmTimeoutEvent*>(e);

        // Timeout was not intended for me...
        if(timeEvent->getIdentity() != this)
            return false;

        
        m_pending = false;
        m_pending_id = -1;

        return true;
        
    } else // Unknown event
    {
        return false;
    }
        
    
    return false;
}

void CombinedTransition::onTransition(QEvent *)
{
    /*
    // Stop
    foreach(&tr, this->parent)
    {

    }
    */
}
