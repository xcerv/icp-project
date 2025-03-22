/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_condition.cpp
 * @author xcervia00
 *
 * @brief Implementation of Condition class
 *
 */

using namespace std;

#include <iostream>
#include <string>
#include "fsm_condition.h"


FsmCondition::FsmCondition(string cond):
    condition{cond}
{
}

const string &FsmCondition::getCondition() const
{
    return this->condition;
}

bool FsmCondition::setCondition(string cond)
{
    this->condition = cond;
    return true;
}

bool FsmCondition::evaluate()
{
    return false; // Todo
}
