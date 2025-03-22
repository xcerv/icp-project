/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_transition.cpp
 * @author xcervia00
 *
 * @brief Implementation of Transition class
 *
 */

using namespace std;

#include <iostream>
#include <string>

#include "model/fsm_state/fsm_state.h"
#include "model/fsm_condition/fsm_condition.h"

#include "fsm_transition.h"

FsmTransition::FsmTransition(size_t src, size_t dest, string cond):
    source{src},
    dest{dest},
    transCondition{cond}
{
}

const size_t &FsmTransition::getSource() const
{
    return this->source;
}

const size_t &FsmTransition::getDestination() const
{
    return this->dest;
}

const string &FsmTransition::getCondition() const
{
    return this->transCondition.getCondition();
}

bool FsmTransition::setSource(size_t state)
{
    this->source = state;
    return true;
}

bool FsmTransition::setDestination(size_t state)
{
    this->dest = state;
    return true;
}

bool FsmTransition::setCondition(string condition)
{
    return this->transCondition.setCondition(condition);
}
