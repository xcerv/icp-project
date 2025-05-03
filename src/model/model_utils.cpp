/**
 * Project name: ICP Project 2024/2025
 *
 * @file model_utils.cpp
 * @author xcervia00
 *
 * @brief Implementation of Model utility functions
 *
 */

#include <stdexcept>

#include <QtAlgorithms>
#include <QVariant>
#include <QDateTime>
#include <QRegularExpression>
#include <QDebug>
#include "mvc_interface.h"
#include "model.h"
#include "combined_event.h"

#include <QtWidgets>

using namespace std;


template <typename Key, typename Value>
void hashmapToString(const QHash<Key, Value> &container, QString &out)
{
    QTextStream stream(&out);
    for (auto it = (container).constBegin(); it != (container).constEnd(); it++)
    {
        stream << QStringLiteral("\t") << it.key();

        if constexpr (std::is_same_v<Value, QVariant>) {
            if(!it.value().toString().isEmpty()){
                stream << QStringLiteral("\t=\t") << it.value().toString();
            }
        }
        else {
            if(!it.value().isEmpty()){
                stream << QStringLiteral("\t=\t") << it.value();
            }
        } 
        
        stream << "\n";
    }
}

void FsmModel::registerView(FsmInterface *view)
{
    this->view = view;
}

QAbstractState *FsmModel::getActiveState() const
{
    return this->machine.initialState();
}

size_t FsmModel::getUniqueTransitionId()
{
    return ++(this->uniqueTransId);
}

size_t FsmModel::getUniqueTransitionId(size_t id)
{
    return id == 0 ? this->getUniqueTransitionId() : id;
}

QStateMachine *FsmModel::getMachine()
{
    return &this->machine;
}

void FsmModel::log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const
{
    (void)time;
    qInfo() << "\nActive State: " << state << "\n" 
            << "Inputs: " << "\n" << qUtf8Printable(varInputs)
            << "Outputs: " << "\n" << qUtf8Printable(varOutputs)
            << "Internals: " << "\n" << qUtf8Printable(varInternals);

    return; // Null operation for model?
}

void FsmModel::log() const
{
    
    QString inputs;
    QString outputs;
    QString internals;
    QVariant test;
    
    hashmapToString(this->varsInput, inputs);
    hashmapToString(this->varsOutput, outputs);
    hashmapToString(this->varsInternal, internals);

    this->log("",
            this->getActiveName(),
            inputs,
            outputs,
            internals
            );
}

void FsmModel::cleanup()
{
    qWarning() << "Performing cleanup...";

    // Unlink states from FSM
    for (ActionState* st : states.values()) {
        if (st != nullptr) {
            machine.removeState(st);
        }
    }

    /** @todo Check that the removal here is correct; state should have ownership of its transition and will delete them too... probably */
    qDeleteAll(states);
    states.clear();
    transitions.clear();

    varsInternal.clear();
    varsInput.clear();
    varsOutput.clear();

    // Reset transition unique id;
    this->uniqueTransId = 0;

    view->cleanup();
}

void FsmModel::throwError(FsmErrorType errNum)
{
    // Some internal handling prior?
    view->throwError(errNum);
}

void FsmModel::throwError(FsmErrorType errNum, const QString &errMsg)
{
    view->throwError(errNum, errMsg);
}

void FsmModel::outputEvent(const QString &outName)
{
    qInfo() << "Fired output event " << outName << ": " << this->varsOutput.value(outName);
    view->outputEvent(this->varsOutput.value(outName));
}

void FsmModel::inputEvent(const QString &name, const QString &value)
{
    // Accept events only if interpretation is running
    if(!this->machine.isRunning()){
        qWarning() << "Caught input event " << name << " of value: " << value << " while FSM is inactive";
        return;
    }

    // Check if given input variable exists
    auto it = this->varsInput.find(name);
    
    // The input variable exits!
    if(it != this->varsInput.end())
    {
        qInfo() << "Caught input event " << name << " of value: " << value;

        // Update value
        this->updateVarInput(name, value);

        // Fire event
        this->machine.postEvent(new FsmInputEvent(name));
    }
}

bool FsmModel::checkValidFormat(const QString &str, const char *regex)
{
    return QRegularExpression(regex).match(str).hasMatch();
}

const QString FsmModel::getActiveName() const
{
    auto active = this->getActiveState();

    // No states yet
    if(active == nullptr) 
        return "No Active State";

    return active->objectName();
}

bool FsmModel::emptyStates() const
{
    return this->getActiveState() == nullptr;
}
