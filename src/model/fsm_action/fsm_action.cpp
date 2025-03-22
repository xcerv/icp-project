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

FsmAction::FsmAction()
{

}

bool FsmAction::execute()
{
    return false;
}

size_t FsmAction::getId()
{
    return size_t();
}

string FsmAction::getAction()
{
    return string();
}

bool FsmAction::setAction(string action)
{
    return false;
}
