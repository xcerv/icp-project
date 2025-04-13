/**
* Project name: ICP Project 2024/2025
*
* @file script_helper.cpp
* @author  xcervia00
*
* @brief An implementation of class for accessing/setting variables from within QJSEngine
*
*/


#include "script_helper.h"
#include "action_state.h"
#include "model.h"

ScriptHelper::ScriptHelper(FsmModel *model, QObject *parent)
    :
    QObject{parent},
    m_model{model}
{
    Q_ASSERT(m_model); // Not necessary, but just to be safe
}

QJSValue ScriptHelper::getInternal(const QString &name)
{
    if(!m_model->varsInternal.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
        return QJSValue(QJSValue::UndefinedValue);
    }
    
    return m_model->engine.toScriptValue(m_model->varsInternal.value(name));
}

bool ScriptHelper::setInternal(const QString &name, const QVariant &value)
{
    if(!m_model->varsInternal.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined internal variable: " + name);
        return false;
    }
    m_model->updateVarInternal(name, value);

    return true;
}

QJSValue ScriptHelper::getInput(const QString &name)
{
    if(!m_model->varsInput.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
        return QJSValue(QJSValue::UndefinedValue);
    }

    return m_model->engine.toScriptValue(m_model->varsInput.value(name));
}

bool ScriptHelper::setInput(const QString &name, const QString &value)
{
    if(!m_model->varsInput.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined input: " + name);
        return false;
    }
    m_model->updateVarInput(name, value);
    return true;
}

QJSValue ScriptHelper::getOutput(const QString &name)
{
    if(!m_model->varsOutput.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
        return QJSValue(QJSValue::UndefinedValue);
    }

    return m_model->engine.toScriptValue(m_model->varsOutput.value(name));
}

bool ScriptHelper::setOutput(const QString &name, const QString &value)
{
    if(!m_model->varsOutput.contains(name))
    {
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined output: " + name);
        return false;
    }

    m_model->updateVarOutput(name, value);
    return true;
}

/*
============================
    PREDEF. JS FUNCTIONS
============================
*/

void ScriptHelper::output(const QString &name, const QJSValue &value)
{
    if(this->setOutput(name, value.toString())){
        m_model->outputEvent(name);
    }
}

void ScriptHelper::set(const QString &name, const QJSValue &value)
{
    if(m_model->varsInternal.contains(name)){
        this->setInternal(name, value.toVariant());
    }
    else{
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: set - Access to undefined variable: " + name);
        return;
    }
}

QJSValue ScriptHelper::valueof(const QString &name)
{
    if(m_model->varsInternal.contains(name)){
        return m_model->engine.toScriptValue(this->m_model->varsInternal.value(name));
    }
    else if (m_model->varsInput.contains(name)){
        return QJSValue(this->m_model->varsInput.value(name));
    }
    else if(m_model->varsOutput.contains(name))
    {
        return QJSValue(this->m_model->varsOutput.value(name));
    }
    else{
        this->m_model->interpretationError(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: valueof - Access to undefined variable: " + name);
        return QJSValue(QJSValue::UndefinedValue);
    }
}

bool ScriptHelper::defined(const QString &name)
{
    return m_model->varsInternal.contains(name) 
            || m_model->varsInput.contains(name)
            || m_model->varsOutput.contains(name);
}

qint64 ScriptHelper::elapsed()
{
    return static_cast<ActionState*>(m_model->getActiveState())->getElapsed();
}

void ScriptHelper::engine_error(const QJSValue &errNum, const QString &errMsg)
{
    this->m_model->interpretationError(static_cast<FsmErrorType>(errNum.toInt()), errMsg);
}

