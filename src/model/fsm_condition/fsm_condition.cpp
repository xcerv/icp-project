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

FsmCondition::FsmCondition()
{
}

size_t FsmCondition::getId()
{
    return size_t();
}

string FsmCondition::getCondition()
{
    return string();
}

bool FsmCondition::setCondition(string cond)
{
    return false;
}

bool FsmCondition::evaluate()
{
    return false;
}
