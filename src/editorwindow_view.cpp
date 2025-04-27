/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow_view.cpp
 * @author  xkadlet00
 *
 * @brief Implementation of view (mvc interface) functions used by the editorwindow
 *
 */

#include "editorwindow.h"
#include "mvc_interface.h"
#include <QObject>
#include <QString>
#include <QPoint>
#include <memory>

void EditorWindow::updateState(const QString &name, const QPoint &pos)
{
}

void EditorWindow::updateStateName(const QString &oldName, const QString &newName)
{
}

void EditorWindow::updateAction(const QString &parentState, const QString &action)
{
}

void EditorWindow::updateActiveState(const QString &name)
{
}

void EditorWindow::updateCondition(size_t transitionId, const QString &condition)
{
}

void EditorWindow::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
}

void EditorWindow::updateVarInput(const QString &name, const QString &value)
{
}

void EditorWindow::updateVarOutput(const QString &name, const QString &value)
{
}

void EditorWindow::destroyState(const QString &name)
{
}

void EditorWindow::destroyAction(const QString &parentState)
{
}

void EditorWindow::destroyCondition(size_t transitionId)
{
}

void EditorWindow::destroyTransition(size_t transitionId)
{
}

void EditorWindow::destroyVarInput(const QString &name)
{
}

void EditorWindow::destroyVarOutput(const QString &name)
{
}

void EditorWindow::destroyVarInternal(const QString &name)
{
}

void EditorWindow::loadFile(const QString &filename)
{
}

void EditorWindow::saveFile(const QString &filename)
{
}

void EditorWindow::renameFsm(const QString &name)
{
}

void EditorWindow::updateVarInternal(const QString &name, const QVariant &value)
{
}


void EditorWindow::cleanup(){} // Clear the class entirely

void EditorWindow::throwError(FsmErrorType errNum)
{
    return; // Nop?
}

void EditorWindow::throwError(FsmErrorType errNum, const QString &errMsg)
{
    return; // Nop?
}

void EditorWindow::outputEvent(const QString &outName)
{
    // Output msg to some output window
}

void EditorWindow::inputEvent(const QString &name, const QString &value)
{
    return; // Nop?
}

void EditorWindow::log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const
{
}

void EditorWindow::log() const
{
    // Here will be the the visualisation of the state handled
    return;
}

void EditorWindow::startInterpretation()
{
    return; // NOP?
}

void EditorWindow::stopInterpretation()
{
    return; // NOP?
}

void EditorWindow::registerModel(std::shared_ptr<FsmInterface> model)
{
    this->model = std::move(model); // todo: can own each other?
}