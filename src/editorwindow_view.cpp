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
#include <QTimer>
#include <QMessageBox>

void EditorWindow::updateState(const QString &name, const QPoint &pos)
{
    if(allStates.contains(name)){
        allStates[name]->setPosition(pos);
    }else{
        insertFSMState(pos, name);
    }
}

void EditorWindow::updateStateName(const QString &oldName, const QString &newName)
{
    StateFSMWidget *w = allStates[oldName];
    w->setName(newName);
    allStates.remove(oldName);
    allStates.insert(newName,w);
}

void EditorWindow::updateAction(const QString &parentState, const QString &action)
{
    allStates[parentState]->setOutput(action);
}

void EditorWindow::updateActiveState(const QString &name)
{
    if(activeState != nullptr){
        activeState->recolor("#b3d1ff","navy");
    }
    activeState = allStates[name];
    activeState->recolor("red","white");
}

void EditorWindow::updateCondition(size_t transitionId, const QString &condition)
{
}

void EditorWindow::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
}

void EditorWindow::updateVarInput(const QString &name, const QString &value)
{
    updateVar(INPUTV, name, value);
}

void EditorWindow::updateVarOutput(const QString &name, const QString &value)
{
    updateVar(OUTPUTV, name, value);
}


void EditorWindow::updateVar(enum variableType type, const QString &name, const QString &value){
    if(allVars[type].contains(name)){
        statusBarLabel->setText("changed value of variable: " + name);
        FSMVariable toDel = allVars[type][name];
        delete toDel.name;
        delete toDel.value;
    }
    allVars[type].insert(name, variablesDisplay->insertVariable(type, name, value));
    variablesDisplay->setActButtons(true,type);
}

void EditorWindow::destroyState(const QString &name)
{
    StateFSMWidget *w = allStates[name];
    if(w == activeState){
        activeState = nullptr;
    }
    w->blockSignals(true);
    QObject::disconnect(w, nullptr, nullptr, nullptr);
    w->setParent(nullptr);
    allStates.remove(name);
    QTimer::singleShot(0, this, [=]() {
        delete w;
    });
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
    destroyVar(INPUTV,name);
}

void EditorWindow::destroyVar(enum variableType type, const QString &name){
    FSMVariable v = allVars[type][name];
    v.name->setParent(nullptr);
    v.value->setParent(nullptr);
    allVars[type].remove(name);
    QTimer::singleShot(0, this, [=]() {
        delete v.name;
        delete v.value;
    });
    if(allVars[type].isEmpty()){
        variablesDisplay->setActButtons(false,type);
    }
}


void EditorWindow::destroyVarOutput(const QString &name)
{
    destroyVar(OUTPUTV,name);
}

void EditorWindow::destroyVarInternal(const QString &name)
{
    destroyVar(INTERNALV, name);
}

void EditorWindow::loadFile(const QString &filename)
{
    return;
}

void EditorWindow::saveFile(const QString &filename)
{
    return;
}

void EditorWindow::renameFsm(const QString &name)
{
    this->setWindowTitle(name);
}

void EditorWindow::updateVarInternal(const QString &name, const QVariant &value)
{
    QString v = value.toString();
    updateVar(INTERNALV, name, v);
}


void EditorWindow::cleanup(){} // Clear the class entirely

void EditorWindow::throwError(FsmErrorType errNum)
{
    return; // Nop?
}

void EditorWindow::throwError(FsmErrorType errNum, const QString &errMsg)
{
    QMessageBox::critical(this,"Error","Err(" + QString::number(errNum) + "): "+errMsg);
}

void EditorWindow::outputEvent(const QString &outName)
{
    // Output msg to some output window
    QMessageBox::information(this,"Information",outName);
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

void EditorWindow::registerModel(FsmInterface *model)
{
    this->model = model; // todo: can own each other?
}
