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
#include <QDebug>
#include <QMessageBox>

void EditorWindow::updateState(const QString &name, const QPoint &pos)
{
    fileModified = true;

    if(allStates.contains(name)){
        allStates[name]->setPosition(pos);
    }else{
        insertFSMState(pos, name);
    }
}

void EditorWindow::updateStateName(const QString &oldName, const QString &newName)
{
    fileModified = true;

    StateFSMWidget *w = allStates[oldName];
    w->setName(newName);
    allStates.remove(oldName);
    allStates.insert(newName,w);
}

void EditorWindow::updateAction(const QString &parentState, const QString &action)
{
    fileModified = true;

    allStates[parentState]->setOutput(action);
}

void EditorWindow::updateActiveState(const QString &name)
{
    fileModified = true;

    // Failed to find the new state
    if(!allStates.contains(name)){
        qCritical() << "VIEW: Failed to find a state to be set to active";
        return;
    }

    if(activeState != nullptr){
        activeState->recolor("#b3d1ff","navy");
    }
    activeState = allStates[name];
    activeState->recolor("red","white");
}

void EditorWindow::updateCondition(size_t transitionId, const QString &condition)
{
    fileModified = true;
}

void EditorWindow::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
    fileModified = true;

    FSMTransition* g = new FSMTransition(workArea);
    g->relocateTransition(allStates[srcState]->getPosition(),allStates[srcState]->getSize(), allStates[destState]->getPosition(), allStates[destState]->getSize());
    g->setDst(destState);
    g->setSrc(srcState);
    g->move(0,0);
    allTransitions[transitionId] = g;
}

void EditorWindow::updateVarInput(const QString &name, const QString &value)
{
    fileModified = true;

    updateVar(INPUTV, name, value);
}

void EditorWindow::updateVarOutput(const QString &name, const QString &value)
{
    fileModified = true;

    updateVar(OUTPUTV, name, value);
}


void EditorWindow::updateVar(enum variableType type, const QString &name, const QString &value){
    fileModified = true;

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
    fileModified = true;

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

    // Disable interpretation if there are no states
    if(allStates.isEmpty())
    {
        this->startButton->setEnabled(false);
    }
}

void EditorWindow::destroyAction(const QString &parentState)
{
    fileModified = true;

    allStates[parentState]->setOutput("");
}

void EditorWindow::destroyCondition(size_t transitionId)
{
    fileModified = true;

}

void EditorWindow::destroyTransition(size_t transitionId)
{
    fileModified = true;
}

void EditorWindow::destroyVarInput(const QString &name)
{
    fileModified = true;

    destroyVar(INPUTV,name);
}

void EditorWindow::destroyVar(enum variableType type, const QString &name){
    fileModified = true;

    // Find variable
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
    fileModified = true;

    destroyVar(OUTPUTV,name);
}

void EditorWindow::destroyVarInternal(const QString &name)
{
    fileModified = true;

    destroyVar(INTERNALV, name);
}

void EditorWindow::loadFile(const QString &filename)
{
    // Nop
    return;
}

void EditorWindow::saveFile(const QString &filename)
{
    // Nop
    return;
}

void EditorWindow::renameFsm(const QString &name)
{
    fileModified = true;
}

void EditorWindow::updateVarInternal(const QString &name, const QVariant &value)
{
    fileModified = true;

    QString v = value.toString();
    updateVar(INTERNALV, name, v);
}


void EditorWindow::cleanup(){
    // Clear the class entirely
    fileModified = false;

   /**
    * @todo CLEANUP
    */
} 

void EditorWindow::throwError(FsmErrorType errNum)
{
    qCritical() << "Error " << errNum << " occured";
    QMessageBox::critical(this,"Error","Err(" + QString::number(errNum) + ")");
    return; // Nop?
}

void EditorWindow::throwError(FsmErrorType errNum, const QString &errMsg)
{
    qCritical() << "Error " << errNum << " is " << errMsg;
    QMessageBox::critical(this,"Error","Err(" + QString::number(errNum) + "): "+errMsg);
}

void EditorWindow::outputEvent(const QString &outName)
{
    // Output msg to some output window
    this->outputEventField->appendPlainText(outName);
}

void EditorWindow::inputEvent(const QString &name, const QString &value)
{
    return; // Nop?
}

void EditorWindow::log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const
{
    (void)time;
    (void)state;
    (void)varInputs;
    (void)varOutputs;
    (void)varInternals;
}

void EditorWindow::log() const
{
    // Probably nop
    return;
}

void EditorWindow::startInterpretation()
{
    return; // NOP?
}

void EditorWindow::stopInterpretation()
{
    this->stopButtonClick();
    return;
}

void EditorWindow::registerModel(FsmInterface *model)
{
    this->model = model;
}
