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


    // Register new functions
    // this->registerFunctions();
}

void ScriptHelper::registerFunctions()
{
    // Left blank - if we decide to abandon "icp." prefix in the script, we may register global functions here directly
    return;
}

QVariant ScriptHelper::getInternal(const QString &name)
{
    if(!m_model->varsInternal.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
    }
    return m_model->varsInternal.value(name);
}

void ScriptHelper::setInternal(const QString &name, const QVariant &value)
{
    if(!m_model->varsInternal.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined internal variable: " + name);
    }
    m_model->updateVarInternal(name, value);
}

QString ScriptHelper::getInput(const QString &name)
{
    if(!m_model->varsInput.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
    }
    return m_model->varsInput.value(name);
}

void ScriptHelper::setInput(const QString &name, const QString &value)
{
    if(!m_model->varsInput.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined input: " + name);
    }
    m_model->updateVarInput(name, value);
}

QString ScriptHelper::getOutput(const QString &name)
{
    if(!m_model->varsOutput.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Access to undefined variable: " + name);
    }
    return m_model->varsOutput.value(name);
}

void ScriptHelper::setOutput(const QString &name, const QString &value)
{
    if(!m_model->varsOutput.contains(name))
    {
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: Attempt to set undefined output: " + name);
    }
    m_model->updateVarOutput(name, value);
}

/*
============================
    PREDEF. JS FUNCTIONS
============================
*/

void ScriptHelper::output(const QString &name, const QJSValue &value)
{
    this->setOutput(name, value.toString());
    m_model->outputEvent(name);
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
        throw FsmModelException(ERROR_INTERPRETATION_EVALUATION, "INTERPRETER: valueof - Access to undefined variable: " + name);
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

