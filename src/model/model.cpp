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
#include <QDebug>

#include "mvc_interface.h"
#include "model.h"
#include "combined_event.h"

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

FsmModel::FsmModel() 
    :
    engine{},
    machine{static_cast<QObject*>(&engine)},
    view{nullptr},
    scriptHelper{this, nullptr},
    uniqueTransId{0}
{
    // Link model to QJSEngine
    QJSValue helperEngine = engine.newQObject(&this->scriptHelper);
    engine.globalObject().setProperty("icp", helperEngine);
}

FsmModel::~FsmModel()
{
    this->cleanup();
}

void FsmModel::updateState(const QString &name, const QPoint &pos)
{
    CATCH_MODEL(
        updateOrInsert(
            this->states, 
            name, 
            // Update
            [&](ActionState *target){
                target->setPosition(pos);
            },
            // New
            [&]() -> ActionState* {
                FORMAT_CHECK_EXCEPTION("MODEL: Invalid state name format", FORMAT_STATE, name);
                auto tmp = new ActionState("", pos);
                tmp->setObjectName(name);
                this->machine.addState(tmp);
                return tmp;
            }
        );
    )

    qInfo() << "MODEL: Updated state " << name << " at position (" << pos.x() << ", " << pos.y() << ")";
    view->updateState(name, pos);
}

void FsmModel::updateStateName(const QString &oldName, const QString &newName)
{
    FORMAT_CHECK("MODEL: Invalid rename state name format", FORMAT_STATE, newName);

    // The state with the new name already exists - ignore this action
    if(this->states.contains(newName))
    {
        view->throwError(ERROR_RENAME_EXISTING);
        return;
    }

    auto it = this->states.find(oldName);
    if(it != states.end()) // Found something
    {
        it.value()->setObjectName(newName);
        states.insert(newName, it.value());
        states.erase(it);
    }
    else // Found nothing - renaming undef state
    {
        view->throwError(ERROR_UNDEFINED_STATE, "MODEL: Tried to rename undefined state");
        return;
    }

    qInfo() << "MODEL: Renamed state " << oldName << " to " << newName;
    view->updateStateName(oldName, newName);
}

void FsmModel::updateAction(const QString &parentState, const QString &action)
{
    // FORMAT_CHECK("MODEL: Invalid updated state name format", FORMAT_ACTION, action); // We just allow everything

    CATCH_MODEL(
        safeUpdate(
            this->states, 
            parentState, 
            // Update
            [&](ActionState *target){
                target->setAction(action);
            },
            {ERROR_UNDEFINED_STATE,
            "MODEL: Failed to update action - No parent state found"}
        );
    )

    qInfo() << "MODEL: Updated action of state " << parentState;
    view->updateAction(parentState, action);
}

void FsmModel::updateActiveState(const QString &name)
{
    CATCH_MODEL(
        safeUpdate(
            this->states, 
            name, 
            // Update
            [&](ActionState *target){
                target->machine()->setInitialState(target);
            },
            {ERROR_UNDEFINED_STATE,
            "MODEL: Failed to update initial state - No parent state found"}
        );
    )

    qInfo() << "MODEL: Set state " << name << " to ACTIVE";
    view->updateActiveState(name);
}

void FsmModel::updateCondition(size_t transitionId, const QString &condition)
{
    CATCH_MODEL(
        safeUpdate(
            this->transitions, 
            transitionId, 
            // Update
            [&](CombinedTransition *target){
                // Try to set condition, error on incorrect format
                target->setCondition(condition);
            },
            {ERROR_UNDEFINED_TRANSITION,
            "MODEL: Failed to update transition condition - No parent state found"}
        );
    )

    qInfo() << "MODEL: Updated condition of transition " << transitionId;
    view->updateCondition(transitionId, condition);
}

void FsmModel::updateTransition(size_t transitionId, const QString &srcState, const QString &destState)
{
    // If transition id is 0, generate a unique id
    transitionId = this->getUniqueTransitionId(transitionId);

    CATCH_MODEL(
        updateOrInsert(
            this->transitions, 
            transitionId, 
            // Update
            [&](CombinedTransition *target){
                // if(srcState == target->targetState) ... swap; // Might use, if we let transitions to change their parents 
                target->setTargetState(safeGetter(states, destState, {ERROR_UNDEFINED_STATE, "MODEL: Failed to obtain destination state"}));
            },
            // New
            [&]() -> CombinedTransition* {
                auto tmp = new CombinedTransition(transitionId);
                safeGetter(states, srcState, {ERROR_UNDEFINED_STATE, "MODEL: Failed to obtain source state"})->addTransition(tmp);
                tmp->setTargetState(safeGetter(states, destState, {ERROR_UNDEFINED_STATE, "MODEL: Failed to obtain destination state"}));
                return tmp;
            }
        );
    )

    qInfo() << "MODEL: Updated transition " << transitionId << " from " << srcState << " to " << destState;
    view->updateTransition(transitionId, srcState, destState);
}

void FsmModel::updateVarInput(const QString &name, const QString &value)
{
    FORMAT_CHECK("MODEL: Invalid Input variable name", FORMAT_VARIABLE, name);
    varsInput.insert(name, value);

    qInfo() << "MODEL: Set input variable " << name << " to " << value;
    view->updateVarInput(name, value);
}

void FsmModel::updateVarOutput(const QString &name, const QString &value)
{
    FORMAT_CHECK("MODEL: Invalid Output variable name", FORMAT_VARIABLE, name);
    varsOutput.insert(name, value);

    qInfo() << "MODEL: Set ouput variable " << name << " to " << value;
    view->updateVarOutput(name, value);
}

void FsmModel::updateVarInternal(const QString &name, const QVariant &value)
{
    FORMAT_CHECK("MODEL: Invalid Internal variable name", FORMAT_VARIABLE, name);
    varsInternal.insert(name, value);

    qInfo() << "MODEL: Set internal variable " << name << " to " << value.toString();
    view->updateVarInternal(name, value);
}

void FsmModel::destroyState(const QString &name)
{
    CATCH_MODEL(
        auto it = safeGetter(this->states, name, {ERROR_UNDEFINED_STATE, "MODULE: Failed to obtain state to destroy"});

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
    )

    qInfo() << "MODEL: Destroyed state" << name;
    view->destroyState(name);
}

void FsmModel::destroyAction(const QString &parentState)
{
    CATCH_MODEL(
        safeGetter(this->states, parentState, {ERROR_UNDEFINED_STATE, "MODEL: Failed to obtain parent state of action to destroy"})->setAction("");
    )

    qInfo() << "MODEL: Destroyed action of state " << parentState;
    view->destroyAction(parentState);
}


void FsmModel::destroyCondition(size_t transitionId)
{
    CATCH_MODEL(
        safeGetter(this->transitions, transitionId, {ERROR_UNDEFINED_TRANSITION,
                 "MODEL: Failed to obtain parent transition of condition to destroy"})->setCondition("");
    )

    qInfo() << "MODEL: Destroyed condition of transition " << transitionId;
    view->destroyCondition(transitionId);
}

void FsmModel::destroyTransition(size_t transitionId)
{
    CATCH_MODEL(
        auto it = safeGetter(this->transitions, transitionId, {ERROR_UNDEFINED_TRANSITION, "MODEL: Failed to obtain transition to destroy"});
        
        // Unregister from state
        if(it->machine() != nullptr)
            {it->machine()->removeTransition(it);} 

        // Remove transition itself
        this->transitions.remove(transitionId); 

        // Free the transition itself
        delete it;
        it = nullptr;
    )

    qInfo() << "MODEL: Destroyed transition " << transitionId;
    view->destroyTransition(transitionId);
}

void FsmModel::destroyVarInput(const QString &name)
{
    this->varsInput.remove(name);

    qInfo() << "MODEL: Destroyed input variable " << name;
    view->destroyVarInput(name);
}

void FsmModel::destroyVarOutput(const QString &name)
{
    this->varsOutput.remove(name);

    qInfo() << "MODEL: Destroyed output variable " << name;
    view->destroyVarOutput(name);
}

void FsmModel::destroyVarInternal(const QString &name)
{
    this->varsInternal.remove(name);

    qInfo() << "MODEL: Destroyed internal variable " << name;
    view->destroyVarInternal(name);
}
