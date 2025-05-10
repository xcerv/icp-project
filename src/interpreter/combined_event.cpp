/**
* Project name: ICP Project 2024/2025
*
* @file combined_event.cpp
* @author  xcervia00
*
* @brief Combined event (inputEvent + async Timeout) used in the FSM application
*
*/

#include "combined_event.h"
#include <QObject>
#include <QStateMachine>
#include <QPointer>
#include <QDebug>

/* Input Event */

FsmInputEvent::FsmInputEvent(const QString &inName)
    : 
    QEvent(FsmInputEvent::getType()),
    name(inName)
{

}

/* Static InputEvent registration */

const QString &FsmInputEvent::getName() const { return this->name; }
QEvent::Type FsmInputEvent::getType() { return FsmInputEvent::m_eventType; }
const QEvent::Type FsmInputEvent::m_eventType = static_cast<QEvent::Type>(QEvent::registerEventType());

/* Timeout Event */

FsmTimeoutEvent::FsmTimeoutEvent(QAbstractTransition *objIdentity)
    : 
    QEvent(FsmTimeoutEvent::getType()),
    m_identity(objIdentity)
{

}

/* Static TimeoutEvent registration */

QEvent::Type FsmTimeoutEvent::getType(){return FsmTimeoutEvent::m_eventType;}
QAbstractTransition* FsmTimeoutEvent::getIdentity(){return m_identity.data();}
const QEvent::Type FsmTimeoutEvent::m_eventType = static_cast<QEvent::Type>(QEvent::registerEventType());
