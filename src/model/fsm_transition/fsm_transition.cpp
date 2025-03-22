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

FsmTransition::FsmTransition()
{
}

size_t FsmTransition::getId()
{
    return size_t();
}

FsmState FsmTransition::getSource()
{
    return FsmState();
}

FsmState FsmTransition::getDestination()
{
    return FsmState();
}

FsmCondition FsmTransition::getCondition()
{
    return FsmCondition();
}

bool FsmTransition::setSource(FsmState state)
{
    return false;
}

bool FsmTransition::setDestination(FsmState state)
{
    return false;
}

bool FsmTransition::setCondition(FsmCondition cond)
{
    return false;
}
