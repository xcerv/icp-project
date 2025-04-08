/**
 * Project name: ICP Project 2024/2025
 *
 * @file model.cpp
 * @author xcervia00
 *
 * @brief Implementation of Model class
 *
 */

#include <stdexcept>

#include <QtAlgorithms>
#include <QVariant>
#include <QDateTime>

#include "mvc_interface.h"
#include "model.h"

using namespace std;


/* Templates */
template <typename Key, typename Value, typename UpdateFunc, typename InsertFunc>
void updateOrInsert(QHash<Key, Value> &container, const Key &key, UpdateFunc &&updater, InsertFunc &&inserter)
{
    auto it = container.find(key);

    if (it != container.end()) {
        // Key exists, call the update
        updater(it.value());
    } else {
        // Key does not exist, create new one
        container.insert(key, inserter());
    }
}

template <typename Key, typename Value, typename UpdateFunc>
void safeUpdate(QHash<Key, Value> &container, const Key &key, UpdateFunc &&updater, const char* errorMsg)
{
    auto it = container.find(key);

    if (it != container.end()) {
        // Key exists, call the update
        updater(it.value());
    } else {
        // Key does not exist, throw error
        throw std::runtime_error(errorMsg);
    }
}

template <typename Key, typename Value>
Value safeGetter(const QHash<Key, Value> &container, const Key &key, const char* errorMsg)
{
    auto it = container.find(key);

    if (it != container.end()) {
        // Key exists, call the update
        return it.value();
    } else {
        // Key does not exist, throw error
        throw std::runtime_error(errorMsg);
    }
}


FsmModel::FsmModel() 
    :
    machine{&engine},
    view{nullptr}
{
    // ...
}

void FsmModel::updateState(const QString &name, const QPoint &pos)
{
    updateOrInsert(
        this->states, 
        name, 
        // Update
        [&](ActionState *target){
            target->setPosition(pos);
        },
        // New
        [&]() -> ActionState* {
            auto tmp = new ActionState("", pos);
            this->machine.addState(tmp);
            return tmp;
        }
    );

    view->updateState(name, pos);
}

void FsmModel::updateAction(const QString &parentState, const QString &action)
{
    safeUpdate(
        this->states, 
        parentState, 
        // Update
        [&](ActionState *target){
            target->setAction(action);
        },
        "MODEL: Failed to update action - No parent state found"
    );

    view->updateAction(parentState, action);
}

void FsmModel::updateInitialState(const QString &name)
{
    safeUpdate(
        this->states, 
        name, 
        // Update
        [&](ActionState *target){
            target->machine()->setInitialState(target);
        },
        "MODEL: Failed to update initial state - No parent state found"
    );

    view->updateInitialState(name);
}

void FsmModel::updateCondition(size_t transitionId, const QString &condition)
{
    safeUpdate(
        this->transitions, 
        transitionId, 
        // Update
        [&](CombinedTransition *target){
            target->setCondition(condition);
        },
        "MODEL: Failed to update initial state - No parent state found"
    );

    view->updateCondition(transitionId, condition);
}

void FsmModel::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
    updateOrInsert(
        this->transitions, 
        transitionId, 
        // Update
        [&](CombinedTransition *target){
            // if(srcState == target->targetState) ... swap; // Might use, if we let transitions to change their parents 
            target->setTargetState(safeGetter(states, destState, "MODEL: Failed to obtain destination state"));
        },
        // New
        [&]() -> CombinedTransition* {
            auto tmp = new CombinedTransition(transitionId);
            safeGetter(states, srcState, "MODEL: Failed to obtain source state")->addTransition(tmp);
            tmp->setTargetState(safeGetter(states, destState, "MODEL: Failed to obtain destination state"));
            return tmp;
        }
    );

    view->updateTransition(transitionId, srcState, destState);
}

void FsmModel::updateVarInput(const QString &name, const QString &value)
{
    varsInput.insert(name, value);
    view->updateVarInput(name, value);
}

void FsmModel::updateVarOutput(const QString &name, const QString &value)
{
    varsOutput.insert(name, value);
    view->updateVarOutput(name, value);
}

void FsmModel::updateVarInternal(const QString &name, const QVariant &value)
{
    varsInternal.insert(name, value);
    view->updateVarInternal(name, value);
}

void FsmModel::destroyState(const QString &name)
{
    auto it = safeGetter(this->states, name, "MODULE: Failed to obtain state to destroy");

    // Destroy all transitions attached to this state
    for (auto &trans : it->transitions()) {
        destroyTransition(static_cast<CombinedTransition*>(trans)->getId());
    }

    // Unregister from state machine
    if(it->machine() != nullptr)
        {it->machine()->removeState(it);}
    
    // Remove from states list
    this->states.remove(name);

    // Delete the state itself
    delete it;
    it = nullptr;

    view->destroyState(name);
}

void FsmModel::destroyAction(const QString &parentState)
{
    safeGetter(this->states, parentState, "MODEL: Failed to obtain parent state of action to destroy")->setAction("");
    view->destroyAction(parentState);
}


void FsmModel::destroyCondition(size_t transitionId)
{
    safeGetter(this->transitions, transitionId, "MODEL: Failed to obtain parent transition of condition to destroy")->setCondition("");
    view->destroyCondition(transitionId);
}

void FsmModel::destroyTransition(size_t transitionId)
{
    auto it = safeGetter(this->transitions, transitionId, "MODEL: Failed to obtain transition to destroy");
    
    // Unregister from state
    if(it->machine() != nullptr)
        {it->machine()->removeTransition(it);} 

    // Remove transition itself
    this->transitions.remove(transitionId); 

    // Free the transition itself
    delete it;
    it = nullptr;

    view->destroyTransition(transitionId);
}

void FsmModel::destroyVarInput(const QString &name)
{
    this->varsInput.remove(name);
    view->destroyVarInput(name);
}

void FsmModel::destroyVarOutput(const QString &name)
{
    this->varsOutput.remove(name);
    view->destroyVarOutput(name);
}

void FsmModel::destroyVarInternal(const QString &name)
{
    this->varsInternal.remove(name);
    view->destroyVarInternal(name);
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

    view->cleanup();
}

void FsmModel::throwError(int errnum)
{
    // Some internal handling prior?
    view->throwError(errnum);
}

void FsmModel::registerView(shared_ptr<FsmInterface> view)
{
    this->view = move(view);
}
