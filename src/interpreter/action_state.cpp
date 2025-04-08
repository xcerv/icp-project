/**
* Project name: ICP Project 2024/2025
*
* @file action_state.cpp
* @author  xcervia00
*
* @brief Extension of QState to allow for storage and execution of Actions
*
*/

#include <QObject>
#include <QJSEngine>

#include "action_state.h"
#include "combined_transition.h"
#include "combined_event.h"

ActionState::ActionState(const QString &action, const QPoint &position) 
    :
    m_action{action},
    m_position{position}
{

}

void ActionState::onEntry(QEvent *event)
{
    (void)event;
    this->executeAction();
    this->machine()->postEvent(new FsmInputEvent("")); // Upon entry, implicitlly fire 'empty' input
}

void ActionState::executeAction()
{
    // Evaluate this action using m_scriptEngine...
}

QJSEngine *ActionState::m_scriptEngine() const
{
    return (this->parent() != nullptr) ? static_cast<QJSEngine*>(this->parent()->parent()) : nullptr;
}

bool ActionState::setPosition(const QPoint &position)
{
    this->m_position = position;
    return true;
}

const QPoint &ActionState::getPosition() const
{
    return this->m_position;
}

bool ActionState::setAction(const QString &action)
{
    this->m_action = action;
    return true;
}

const QString &ActionState::getAction() const
{
    return this->m_action;
}
