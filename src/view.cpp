/**
 * Project name: ICP Project 2024/2025
 *
 * @file view.cpp
 * @author xlogin00
 *
 * @brief Implementation of View class
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"
#include "view.h"

using namespace std;

// Customise accordingly
FsmView::FsmView():
    model{nullptr}
{
    // ...
}

// Interface methods


void FsmView::updateState(const QString &name, const QPoint &pos)
{
}

void FsmView::updateStateName(const QString &oldName, const QString &newName)
{
}

void FsmView::updateAction(const QString &parentState, const QString &action)
{
}

void FsmView::updateInitialState(const QString &name)
{
}

void FsmView::updateCondition(size_t transitionId, const QString &condition)
{
}

void FsmView::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
}

void FsmView::updateVarInput(const QString &name, const QString &value)
{
}

void FsmView::updateVarOutput(const QString &name, const QString &value)
{
}

void FsmView::destroyState(const QString &name)
{
}

void FsmView::destroyAction(const QString &parentState)
{
}

void FsmView::destroyCondition(size_t transitionId)
{
}

void FsmView::destroyTransition(size_t transitionId)
{
}

void FsmView::destroyVarInput(const QString &name)
{
}

void FsmView::destroyVarOutput(const QString &name)
{
}

void FsmView::destroyVarInternal(const QString &name)
{
}

void FsmView::loadFile(const QString &filename)
{
}

void FsmView::saveFile(const QString &filename)
{
}

void FsmView::renameFsm(const QString &name)
{
}

void FsmView::updateVarInternal(const QString &name, const QVariant &value)
{
}


void FsmView::cleanup(){} // Clear the class entirely

void FsmView::throwError(FsmErrorType errNum)
{
    return; // Nop?
}

void FsmView::throwError(FsmErrorType errNum, const QString &errMsg)
{
    return; // Nop?
}

void FsmView::outputEvent(const QString &outName)
{
    // Output msg to some output window
}

void FsmView::log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const
{
}

void FsmView::log() const
{
    // Here will be the the visualisation of the state handled
    return;
}

void FsmView::startInterpretation()
{
    return; // NOP?
}

void FsmView::stopInterpretation()
{
    return; // NOP?
}

void FsmView::registerModel(shared_ptr<FsmInterface> model)
{
    this->model = move(model);
}