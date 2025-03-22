/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_state.cpp
 * @author xcervia00
 *
 * @brief Implementation of State class
 *
 */

#include <unordered_set>
#include <vector>
#include "fsm_state.h"

/**
 * @brief
 * @param n
 * @param p
 * @param t
 */
FsmState::FsmState(string n, FsmPoint p, stateType t):
    name{n},
    position{p},
    type{t}
{
}

const string &FsmState::getName() const
{
    return this->name;
}

const FsmPoint &FsmState::getPosition() const
{
    return this->position;
}

const stateType &FsmState::getType() const
{
    return this->type;
}

const size_t &FsmState::getActionByOrder(size_t order) const
{
    return this->actions.at(order);
}

const vector<size_t> &FsmState::getActions()
{
    return this->actions;
}

const unordered_set<size_t> &FsmState::getTransitions()
{
    return this->transitions;
}

bool FsmState::setName(string name)
{
    this->name = name;
    return true;
}

bool FsmState::setPosition(FsmPoint position)
{
    this->position = position;
    return true;
}

bool FsmState::setType(stateType type)
{
    this->type = type;
    return true;
}

bool FsmState::addTransition(size_t id)
{
    if(this->transitions.find(id) == this->transitions.end()){ // Inserted new
        this->transitions.emplace(id);
        return true;
    }
    else{ // Inserted nothing
        return false;
    }
}

bool FsmState::removeTransition(size_t id)
{
    if(this->transitions.find(id) != this->transitions.end()){ // Removed element
        this->transitions.erase(id);
        return true;
    }
    else{ // Removed nothing
        return false;
    }
}


bool FsmState::addAction(size_t id, size_t currOrder, size_t order)
{
    // Already at right position
    if(order == currOrder && order<actions.size() && actions[order] == id) return true;

    // id is present already
    if(currOrder != SIZE_MAX)
    {
        actions.erase(actions.begin() + currOrder);
    }

    actions.insert(actions.begin() + order, id);

    return true;
}

bool FsmState::removeAction(size_t order)
{
    actions.erase(actions.begin()+order);

    return true;
}
