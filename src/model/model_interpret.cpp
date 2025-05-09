/**
 * Project name: ICP Project 2024/2025
 *
 * @file model_interpret.cpp
 * @author xcervia00
 *
 * @brief The interpretation portion of Model implementation
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"
#include "model.h"

#include <QtGlobal>
#include <QCoreApplication>
#include <QDebug>
#include <QStateMachine>

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    #define STOP_EVALUATION(engine) do{(engine).throwError("Interpretation error");}while(0)
#else
    #define STOP_EVALUATION(engine) 
#endif

void FsmModel::renameFsm(const QString &name)
{
    this->machine.setObjectName(name);
    view->renameFsm(name);
}

void FsmModel::startInterpretation()
{
    // If there are no states or no active state, exit
    if(this->emptyStates()){
        qCritical() << "INTERPRETATION: Attempt to interpret empty state machine";
        this->view->stopInterpretation();
        return;
    }

    // Log before interpretation
    qInfo() << "Interpretation started...";
    this->log();

    // Backup 
    backup.vInternal = this->varsInternal;
    backup.vInput = this->varsInput;
    backup.vOutput = this->varsOutput;
    backup.initialState = this->machine.initialState();

    // By default, no state is 'last' until one is entered
    ActionState::setLastState(nullptr);

    this->machine.start();
    return;
}

void FsmModel::interpretationError(FsmErrorType errNum)
{
    STOP_EVALUATION(this->engine);
    this->stopInterpretation();
    this->throwError(errNum);
}

void FsmModel::interpretationError(FsmErrorType errNum, const QString &errMsg)
{
    STOP_EVALUATION(this->engine);
    this->stopInterpretation();
    this->throwError(errNum, errMsg);
}

void FsmModel::restoreInterpretationBackup()
{
    // Backup not set
    if(backup.initialState == nullptr)
        return;

    // Restore internal vars
    for (const auto &key : backup.vInternal.keys()) 
    {
        this->updateVarInternal(key, backup.vInternal.value(key));
    }
    // Restore input vars
    for (const auto &key : backup.vInput.keys()) 
    {
        this->updateVarInput(key, backup.vInput.value(key));
    }
    // Restore output vars
    for (const auto &key : backup.vOutput.keys()) 
    {
        this->updateVarOutput(key, backup.vOutput.value(key));
    }

    // Restore initial state
    this->updateActiveState(backup.initialState->objectName());
}

void FsmModel::stopInterpretation()
{
    if(!machine.isRunning()){
        return;
    }

    qInfo() << "Interpretation stopped...";

    // Stop the machine immediatelly
    this->machine.stop();

    // On full stop restore original values
    this->restoreInterpretationBackup();

    // Stop unprocessed delayed transition events
    for(auto &tr : this->transitions)
    {
        tr->stopTimer();
    }

    // Request cleanup; optional
    this->engine.collectGarbage();
    return;
}
