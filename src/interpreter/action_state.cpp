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
#include <QDebug>

#include "action_state.h"
#include "combined_transition.h"
#include "combined_event.h"

ActionState::ActionState(const QString &action, const QPoint &position) 
    :
    m_action{action},
    m_position{position},
    m_timeVisited{},
    m_timeSinceEntry{}
{
    
}

void ActionState::onEntry(QEvent *event)
{
    (void)event;

    // If state was changed, update timer
    if(this != ActionState::getLastState())
    {
        ActionState::setLastState(this);
        m_timeVisited.start();
    }

    // Always reset this timer on any state entry
    m_timeSinceEntry.start();

    qInfo() << "Interpreter: State entered: " << this->objectName();

    // Trigger action of the state
    this->executeAction();
    if(this->machine()->isRunning()){
        this->machine()->postEvent(new FsmInputEvent("")); // Upon entry, implicitlly fire 'empty' input
    }
}

void ActionState::executeAction()
{
    // Get the engine for evaluation
    QJSEngine* engine = static_cast<QJSEngine*>(this->machine()->parent());
    
    //auto result = engine->evaluate(QString("with (icp) { %1 }").arg(this->getAction())); // Optionally remove icp. prefix
    
    // Evaluate the action
    auto result = engine->evaluate(QStringLiteral("(function(){ %1 })();").arg(this->getAction()));

    if(result.isError()){
        qCritical() << "Intepreter: Error during execution of state action";
    }
}

QJSEngine *ActionState::m_scriptEngine() const
{
    // Get the relative parent (script engine) dynamically
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

bool ActionState::setLastState(ActionState *state)
{
    ActionState::m_lastState = state;
    return true;
}

ActionState *ActionState::getLastState()
{
    return ActionState::m_lastState;
}

qint64 ActionState::getElapsed() const
{
    return m_timeVisited.elapsed();
}

qint64 ActionState::getElapsedSinceEntry() const
{
    return m_timeSinceEntry.elapsed();
}

// By default, last state is nullptr
QPointer<ActionState> ActionState::m_lastState = nullptr;
