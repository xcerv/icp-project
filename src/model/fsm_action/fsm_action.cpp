/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_action.cpp
 * @author xcervia00
 *
 * @brief Implementation of Action class
 *
 */
using namespace std;

#include <iostream>
#include <string>
#include "fsm_action.h"

FsmAction::FsmAction(string act, size_t ord):
    action{act},
    order{ord}
{
}

bool FsmAction::execute()
{
    return false; // Todo
}

const string & FsmAction::getAction() const
{
    return this->action;
}

const size_t & FsmAction::getOrder() const
{
    return this->order;
}

bool FsmAction::setAction(string action)
{
    this->action = action;
    return true;
}

bool FsmAction::setOrder(size_t newOrder)
{
    this->order = newOrder;
    return true;
}
