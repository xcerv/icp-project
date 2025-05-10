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
#include <QDebug>
#include <QObject>
#include <QStateMachine>
#include <QJSEngine>
#include <QRegularExpression>
#include "fsm_exceptions.h"

#include <stdexcept>

CombinedTransition::CombinedTransition(const size_t id) 
    :
    m_pending{false},
    m_pending_id{-1},
    m_id{id}
{
    
}
 
CombinedTransition::CombinedTransition(const QString &name, const QString &guard, const QString &timeout) 
    :
    m_name{name}, 
    m_guard{guard}, 
    m_timeout{timeout},
    m_pending{false},
    m_pending_id{-1},
    m_id{0}
{

}

CombinedTransition::CombinedTransition(const QString &unparsed_condition)
    :
    m_pending{false},
    m_pending_id{-1},
    m_id{0}
{
    this->setCondition(unparsed_condition);
}


bool CombinedTransition::setCondition(const QString &condition)
{
   // Regex parsing of the condition in format: INPUT [COND] @TIMEOUT 
   QRegularExpression re(REGEX_TRANSITION_CONDITION);
   QRegularExpressionMatch match = re.match(condition);

    if(match.hasMatch())
    {
        this->m_name = match.captured(1);
        this->m_guard = match.captured(3);
        this->m_timeout = match.captured(5);
        return true;
    }
    else
    {
        throw FsmModelException(ERROR_CONDITION_FORMAT, "MODEL: Given guard condition has unsupported format");
    }

    return false;
}

size_t CombinedTransition::getId() const
{
    return m_id;
}

void CombinedTransition::stopTimer()
{
    // Cancel any timed events
    if(m_pending && m_pending_id != -1)
        this->machine()->cancelDelayedEvent(this->m_pending_id);

    this->m_pending_id = -1;
    this->m_pending = false;
}

bool CombinedTransition::eventTest(QEvent *e)
{
    if(e == nullptr || !this->machine()->isRunning()) return false;

    if(e->type() == FsmInputEvent::getType()) // Initial input trigger
    {
        // Already waiting for timeout, wait for it instead
        if(m_pending)
            return false;

        // Mismatch in the input name
        if(static_cast<FsmInputEvent*>(e)->getName() != this->m_name)
            return false;

        // Safety check (just do nothing)
        if(this->machine() == nullptr || this->machine()->parent() == nullptr) 
            return false;

        // Try guard condition here...
        if(!m_guard.isEmpty())
        {
            QJSEngine* engine = static_cast<QJSEngine*>(this->machine()->parent()); // Get the parent of main statemachine --> the QJSEngine 
            QJSValue guard_result = engine->evaluate(this->m_guard);
            
            // Has to be bool and that is true
            if(!guard_result.isBool() || !guard_result.toBool())
                return false;
        }

        // Guard passed... start timeout
        int timeoutMs = 0;

        // Timeout is not empty - extract its value
        if(!m_timeout.isEmpty())
        {
            bool ok;
            timeoutMs = this->m_timeout.toInt(&ok);

            // Timeout is not a number ==> Try to evaluate it as a script and expect integer value as output
            if(!ok)
            {
                auto timeoutResult = (static_cast<QJSEngine*>(this->machine()->parent())->evaluate(this->m_timeout));
                
                if(timeoutResult.isNumber())
                {
                    timeoutMs = timeoutResult.toInt();
                }
            }

            if(timeoutMs < 0){timeoutMs = 0;}
        }

        // Start new timeout
        this->m_pending_id = this->machine()->postDelayedEvent(new FsmTimeoutEvent(this), timeoutMs);
        this->m_pending = true;
        return false;

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

        qInfo() << "Interpreter: Transition of id " << this->m_id << " from state " << this->sourceState()->objectName()
                << " to " << this->targetState()->objectName() << " had been triggered";

        return true;
        
    } else // Unknown event
    {
        return false;
    }
        
    
    return false;
}

void CombinedTransition::onTransition(QEvent * e)
{
    // Ignore unexpected events
    if(e == nullptr || e->type() != FsmTimeoutEvent::getType() || static_cast<FsmTimeoutEvent*>(e)->getIdentity() != this)
        return;

    // Parent must exist!
    if(this->parent() == nullptr) 
        return; // Throw exception?

    auto parentState = static_cast<QState*>(this->parent());

    // Stop any pending timers
    foreach(auto &tr, parentState->transitions())
    {
        auto curr = static_cast<CombinedTransition*>(tr);
        curr->stopTimer();
    }
}

QString CombinedTransition::getName() const {
    return m_name;
}

QString CombinedTransition::getGuard() const {
    return m_guard;
}

QString CombinedTransition::getTimeout() const {
    return m_timeout;
}