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

using namespace std;

void FsmModel::renameFsm(const QString &name)
{
    this->machine.setObjectName(name);
    view->renameFsm(name);
}

void FsmModel::startInterpretation()
{
    // Backup 
    backup.vInternal = this->varsInternal;
    backup.vInput = this->varsInput;
    backup.vOutput = this->varsOutput;
    backup.initialState = this->machine.initialState();

    // Set global variables for QJSEngine
    // todo... // this->engine.globalObject

    this->machine.start();
    return;
}

void FsmModel::stopInterpretation()
{
    // On full stop restore original values (maybe add another button for restoring?)
    this->varsInternal = backup.vInternal;
    this->varsInput = backup.vInput;
    this->varsOutput = backup.vOutput;
    this->machine.setInitialState(backup.initialState);

    this->machine.stop();
    return;
}

// Should there be pause method?