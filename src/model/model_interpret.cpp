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
#include <QDebug>

using namespace std;

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

void FsmModel::stopInterpretation()
{
    if(!machine.isRunning()){
        return;
    }

    qInfo() << "Interpretation stopped...";

    this->machine.stop();

    // On full stop restore original values (maybe add another button for restoring?)
    this->varsInternal = backup.vInternal;
    this->varsInput = backup.vInput;
    this->varsOutput = backup.vOutput;
    this->machine.setInitialState(backup.initialState);

    this->engine.collectGarbage();
    return;
}

// Should there be pause method?