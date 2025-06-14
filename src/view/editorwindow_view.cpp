/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow_view.cpp
 * @author  xkadlet00
 * @author  xcervia00
 *
 * @brief Implementation of view (mvc interface) functions used by the editorwindow
 *
 */

#include "editorwindow.h"
#include "mvc_interface.h"
#include <QAction>
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
        for(auto c : allTransitionsUI.keys()){
            if(c.first == name || c.second == name){
                auto src = allStates[c.first];
                auto dst = allStates[c.second];
                if(src && dst){
                    allTransitionsUI[c]->relocateTransition(src->getPosition(),src->getSize(),dst->getPosition(),dst->getSize());
                }
            }
        }
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

    for(auto &c : allTransitionsConditions){
        if(c.src == oldName){
            c.src = newName;
        }
        if(c.dest == oldName){
            c.dest = newName;
        }
    }

    QHash<QPair<QString, QString>, FSMTransition*> updatedTransitions;

    for (auto it = allTransitionsUI.begin(); it != allTransitionsUI.end(); ) {
        QPair<QString, QString> key = it.key();
        FSMTransition* value = it.value();

        QPair<QString, QString> newKey = key;

        bool changed = false;
        if (key.first == oldName) {
            newKey.first = newName;
            changed = true;
        }
        if (key.second == oldName) {
            newKey.second = newName;
            changed = true;
        }

        if (changed) {
            it = allTransitionsUI.erase(it);  // Remove old key
            updatedTransitions[newKey] = value;  // Save to insert later
        } else {
            ++it;
        }
    }

    // Reinsert updated keys
    for (auto it = updatedTransitions.begin(); it != updatedTransitions.end(); ++it) {
        allTransitionsUI[it.key()] = it.value();
    }

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
        activeState->recolor("#b3d1ff","navy","");
    }
    activeState = allStates[name];
    activeState->recolor("navy","white","w");

    if(!isInterpreting){
        this->startButton->setEnabled(true);
        this->startInterpretationAct->setEnabled(true);
    }
}

void EditorWindow::updateCondition(size_t transitionId, const QString &condition)
{
    fileModified = true;
    auto help = allTransitionsConditions[transitionId];
    help.condition = condition;
    allTransitionsConditions[transitionId] = help;
}

void EditorWindow::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
    fileModified = true;
    QPair<QString, QString> key = {srcState, destState};
    QPair<QString, QString> keyR = {destState, srcState};

    if(allTransitionsUI.contains(key)) {
        allTransitionsUI[key]->addTransition(transitionId);
    }else if(allTransitionsUI.contains(keyR)){
        allTransitionsUI[keyR]->addTransition(transitionId);
    }else {
        FSMTransition* g = new FSMTransition(workArea);
        g->relocateTransition(allStates[srcState]->getPosition(),allStates[srcState]->getSize(), allStates[destState]->getPosition(), allStates[destState]->getSize());
        g->move(0,0);
        connect(g,&FSMTransition::editTransition, this, &EditorWindow::editTransitionHanling);
        allTransitionsUI[key] = g;
        allTransitionsUI[key]->addTransition(transitionId);
    }
    allTransitionsConditions[transitionId] = {srcState,destState,""};
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
        FSMVariable &chng = allVars[type][name];
        chng.value->setText(value);
    }else{
        allVars[type].insert(name, variablesDisplay->insertVariable(type, name, value));
        variablesDisplay->setActButtons(true,type);
    }
}

void EditorWindow::destroyState(const QString &name)
{
    fileModified = true;

    StateFSMWidget *w = allStates[name];
    if(w == activeState){
        activeState = nullptr;
        this->startButton->setEnabled(false);
        this->startInterpretationAct->setEnabled(false);
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
    fileModified = true;

    allStates[parentState]->setOutput("");
}

void EditorWindow::destroyCondition(size_t transitionId)
{
    fileModified = true;
    auto help = allTransitionsConditions[transitionId];
    help.condition = "";
    allTransitionsConditions[transitionId] = help;
}

void EditorWindow::destroyTransition(size_t transitionId)
{
    auto help = allTransitionsConditions[transitionId];

    QPair<QString, QString> key  = {help.src, help.dest};
    QPair<QString, QString> keyR = {help.dest, help.src};

    allTransitionsConditions.remove(transitionId);

    FSMTransition * delTr;

    if(allTransitionsUI.contains(key)) {
        delTr = allTransitionsUI[key];
    }else if(allTransitionsUI.contains(keyR)){
        delTr = allTransitionsUI[keyR];
        key = keyR;
    }else{
        //throwError(99,"Internal error occured");
        return;
    }
    delTr->subTransition(transitionId);
    auto num = delTr->getTransitions();
    if (num.isEmpty()){
        delTr->blockSignals(true);
        QObject::disconnect(delTr, nullptr, nullptr, nullptr);
        delTr->setParent(nullptr);
        allTransitionsUI.remove(key);
        QTimer::singleShot(0, this, [=]() {
            delete delTr;
        });
    }
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
    (void)filename;
    return;
}

void EditorWindow::saveFile(const QString &filename)
{
    // Nop
    (void)filename;
    return;
}

void EditorWindow::loadStream(QTextStream &stream)
{
    this->model->loadStream(stream);
    return;
}

void EditorWindow::saveStream(QTextStream &stream)
{
    this->model->saveStream(stream);
    return;
}

void EditorWindow::renameFsm(const QString &name)
{
    setWindowTitle(name);
    fileModified = true;
}

void EditorWindow::updateVarInternal(const QString &name, const QVariant &value)
{
    fileModified = true;

    QString v = value.toString();
    updateVar(INTERNALV, name, v);
    
    QLabel * labelName = this->allVars[INTERNALV].value(name).name;

    switch(value.type())
    {
        case QVariant::Bool:
            labelName->setText("bool " + name);
            break;
        case QVariant::Int:
            labelName->setText("int " + name);
            break;
        case QVariant::String:
            labelName->setText("string " + name);
            break;
        case QVariant::Double:
            labelName->setText("float " + name);
            break;
        default:
            break;
    }
}


void EditorWindow::cleanup(){
    // Clear the class entirely
    fileModified = false;

    activeState = nullptr;

    // Stop any active actions
    this->cancelActionMove();
    this->cancelActionConnect();

    //qDeleteAll(workArea->children());
    qDeleteAll(allStates);
    qDeleteAll(allTransitionsUI);

    allTransitionsUI.clear();
    allStates.clear();

    allTransitionsConditions.clear();

    for(int i =0; i < NUMV; i++){
        for(auto d : allVars[i]){
            delete d.name;
            delete d.value;
        }
        allVars[i].clear();
    }
} 

void EditorWindow::throwError(FsmErrorType errNum)
{
    this->stopButtonClick();

    if(isShowingError) 
        {return;}
    isShowingError = true;

    qCritical() << "Error " << errNum << " occured";
    QMessageBox::critical(this,"Error","Err(" + QString::number(errNum) + ")");

    isShowingError = false;
    return;
}

void EditorWindow::throwError(FsmErrorType errNum, const QString &errMsg)
{
    this->stopButtonClick();

    if(isShowingError) 
        {return;}
    isShowingError = true;

    qCritical() << "Error " << errNum << " is " << errMsg;
    QMessageBox::critical(this,"Error","Err(" + QString::number(errNum) + "): "+errMsg);
    
    isShowingError = false;
    return;
}

void EditorWindow::outputEvent(const QString &outName)
{
    // Output msg to some output window
    this->outputEventField->appendPlainText(outName);
}

void EditorWindow::inputEvent(const QString &name, const QString &value)
{
    this->model->inputEvent(name, value);
    return;
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
    if(isInterpreting == false)
    {
        this->startButtonClick();
    }
}

void EditorWindow::stopInterpretation()
{
    if(isInterpreting == true)
    {
        this->stopButtonClick();
    }
}

void EditorWindow::restoreInterpretationBackup()
{
    // Nop?
    return;
}

void EditorWindow::registerModel(FsmInterface *model)
{
    this->model = model;
}
