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
 
 using namespace std;
 

void FsmModel::registerView(FsmInterface *view)
{
    this->view = view;
}

QAbstractState *FsmModel::getActiveState() const
{
    return (*this->machine.configuration().begin());
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
    (void)state;
    (void)varInputs;
    (void)varOutputs;
    (void)varInternals;

    return; // Null operation for model?
}

void FsmModel::log() const
{
    // Deprecate this below: Just let view take its internal representation rather than pass ours
    // view->log(QDateTime::currentDateTime().toString(), (*this->machine.configuration().begin())->objectName(),
    view->log();
}

void FsmModel::cleanup()
{
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
    qDebug() << "ERRNUM:" << errNum << "ERRMSG:" << errMsg;
    view->throwError(errNum, errMsg);
}

void FsmModel::outputEvent(const QString &outName)
{
    qDebug() << outName << ": " << this->varsOutput.value(outName);
    view->outputEvent(outName);
}

void FsmModel::inputEvent(const QString &name, const QString &value)
{
    // Accept events only if interpretation is running
    if(!this->machine.isRunning())
        return;

    // Check if given input variable exists
    auto it = this->varsInput.find(name);
    
    // The input variable exits!
    if(it != this->varsInput.end())
    {
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
