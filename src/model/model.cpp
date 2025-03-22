/**
 * Project name: ICP Project 2024/2025
 *
 * @file model.cpp
 * @author xcervia00
 *
 * @brief Implementation of Model class
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>

#include "mvc_interface.h"
#include "model.h"

using namespace std;

FsmModel::FsmModel():
    name{"unnamed"},
    view{nullptr},
    currentState{0}
{
    // ...
}

// Interface methods
void FsmModel::updateState(size_t id, string name, FsmPoint pos, stateType type)
{
    auto it = states.find(id);

    if(it != states.end()) // Updating old
    {
        it->second.setName(name);
        it->second.setPosition(pos);
        it->second.setType(type);
    }
    else // Inserting new
    {
        states.emplace(id, FsmState(name, pos, type));
    }

    // Update the view 
    view->updateState(id, name, pos, type);
}

void FsmModel::updateAction(size_t id, size_t parent_state_id, size_t order, string action)
{

    // Update action
    auto it = actions.find(id);
    size_t oldOrder = ID_UNSET;

    if(it != actions.end())
    {
        it->second.setAction(action);
        oldOrder = it->second.getOrder();
        it->second.setOrder(order);
    }
    else
    {
        actions.emplace(id, FsmAction(action, order));
    }
    
    // Update parent state
    auto it_st = states.find(parent_state_id);
    if(it_st != states.end())
    {
        it_st->second.addAction(id, oldOrder, order);
    }
    else
    {
        throw runtime_error("MODEL: Attempted to set action of non-existent state");
    }

    // Update the view 
    view->updateAction(id, parent_state_id, order, action);
}


void FsmModel::updateCondition(size_t parent_transition_id, string condition)
{
    auto it = transitions.find(parent_transition_id);

    if(it != transitions.end())
    {
        it->second.setCondition(condition);
    }
    else
    {
        throw runtime_error("MODEL: Attempting to set condition of non-existant transition");
    }

    view->updateCondition(parent_transition_id, condition);
}

void FsmModel::updateTransition(size_t id, size_t id_state_src, size_t id_state_dest)
{
    auto it = transitions.find(id); // See if transition already exists
    auto it_st = states.find(id_state_src); // New source state

    if(it_st == states.end() || states.find(id_state_dest) == states.end()) // Parameter states don't exist ==> error
        {throw runtime_error("MODEL: Transition is not tied to source state");}

    if(it != transitions.end()) // Found existing transition
    {
        if(it->second.getSource() != id_state_src) // Check if source state is changing
        {
            // Unregister from original source state
            auto it_orig_st = states.find(it->second.getSource());
            if(it_orig_st == states.end()) 
                {throw runtime_error("MODEL: Transition wasn't originally tied to an existing state");}
            it_orig_st->second.removeTransition(id);
            
            // Register to new source state
            it_st->second.addTransition(id);
            it->second.setSource(id_state_src);
        }

        it->second.setDestination(id_state_dest);
    }
    else // Create new transition
    {
        transitions.emplace(id, FsmTransition(id_state_src, id_state_dest, ""));
        it_st->second.addTransition(id);
    }

    view->updateTransition(id, id_state_src, id_state_dest);
}

void FsmModel::updateVarInput(size_t id, string name, string value)
{
    auto it = varsInput.find(id);

    if(it != varsInput.end()) // Updating old
    {
        it->second.setName(name);
        it->second.setValue(value);
    }
    else // Inserting new
    {
        varsInput.emplace(id, FsmVariableInput(name, value));
    }

    view->updateVarInput(id, name, value);
}
void FsmModel::updateVarOutput(size_t id, string name, string value)
{
    auto it = varsOutput.find(id);

    if(it != varsOutput.end()) // Updating old
    {
        it->second.setName(name);
        it->second.setValue(value);
    }
    else // Inserting new
    {
        varsOutput.emplace(id, FsmVariableOutput(name, value));
    }

    view->updateVarOutput(id, name, value);
}

void FsmModel::updateVarInternal(size_t id, string name, string value, varType type)
{
    auto it = varsInternal.find(id);

    if(it != varsInternal.end()) // Updating old
    {
        it->second.setName(name);
        it->second.setValue(value);
        it->second.setType(type);
    }
    else // Inserting new
    {
        varsInternal.emplace(id, FsmVariableInternal(name, value, type));
    }

    view->updateVarInternal(id, name, value, type);
}

void FsmModel::destroyState(size_t id)
{
    auto it = states.find(id);
    if(it != states.end()) // Destroying existing
    {
        // Destroy actions tied to state
        for (auto &act : it->second.getActions()) {
            destroyAction(act, id);
        }

        // Destroy transitions tied to state
        for (auto &trans : it->second.getTransitions()) {
            destroyTransition(trans);
        }

        // Destroy state itself
        states.erase(it);
    }
    else
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant state");
    }

    view->destroyState(id);
}

void FsmModel::destroyAction(size_t id, size_t parent_state_id)
{    auto it = actions.find(id);
    if(it != actions.end()) // Destroying existing
    {
        // Unregister from state
        auto it_st = states.find(parent_state_id);
        if(it_st->second.getActionByOrder(it->second.getOrder() != id)){
            throw runtime_error("MODEL: Attempt to unlink action from state with incorrect order");;
        }

        if(it_st != states.end()){
            it_st->second.removeAction(it->second.getOrder());
        }
        else{
            throw runtime_error("MODEL: Attempt to unlink state during action destruction");
        }

        // Remove action itself
        actions.erase(it);
    }
    else
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant state");
    }
    view->destroyAction(id, parent_state_id);
}

void FsmModel::destroyCondition(size_t parent_id)
{
    auto it = transitions.find(parent_id);
    if(it != transitions.end()) // Destroying existing
    {
        it->second.setCondition(""); // Just set to empty string
    }
    else // Somehow the parent transition doesn't exist
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant transition's condition");
    }

    view->destroyCondition(parent_id);
}

void FsmModel::destroyTransition(size_t id)
{
    auto it = transitions.find(id);
    if(it != transitions.end()) // Destroying existing
    {
        // Unlink from source state
        auto it_st = states.find(it->second.getSource());
        if(it_st == states.end())
            {throw runtime_error("MODEL: Attempt to destroy transition with nonexistant source state");}
        
        bool unlink_res = it_st->second.removeTransition(id);

        if (unlink_res == false)
            {throw runtime_error("MODEL: Attempt to unlink transition that was not linked to source state");}

        // Destroy the transition itself
        transitions.erase(it);
    }
    else // Somehow the parent transition doesn't exist
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant transition");
    }

    view->destroyTransition(id);
}

void FsmModel::destroyVarInput(size_t id)
{
    auto it = varsInput.find(id);
    if(it != varsInput.end()) // Destroying existing
    {
        varsInput.erase(it);
    }
    else
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant input variable");
    }

    view->destroyVarInput(id);
}

void FsmModel::destroyVarOutput(size_t id)
{
    auto it = varsOutput.find(id);
    if(it != varsOutput.end()) // Destroying existing
    {
        varsOutput.erase(it);
    }
    else
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant output variable");
    }

    view->destroyVarOutput(id);
}

void FsmModel::destroyVarInternal(size_t id)
{
    auto it = varsInternal.find(id);
    if(it != varsInternal.end()) // Destroying existing
    {
        varsInternal.erase(it);
    }
    else
    {
        throw runtime_error("MODEL: Attempt to destroy non-existant internal variable");
    }

    view->destroyVarInternal(id);
}

void FsmModel::log(string time, string state, string varInputs, string varOutputs, string varInternals)
{
    return; // Null operation for model?
}

void FsmModel::cleanup()
{
    states.clear();
    actions.clear();
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
