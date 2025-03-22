/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_variable.cpp
 * @author xcervia00
 *
 * @brief Implementation of Variable class
 *
 */
using namespace std;

#include <iostream>
#include <string>
#include "fsm_variable.h"

/* GENERIC */
size_t FsmVariable::getId()
{
    return size_t();
}

string FsmVariable::getName()
{
    return string();
}

string FsmVariable::getValue()
{
    return string();
}

bool FsmVariable::setName(string name)
{
    return false;
}

bool FsmVariable::setValue(string value)
{
    return false;
}

/* INTERNAL */
size_t FsmVariableInternal::getId()
{
    return size_t();
}

/* INPUT */
size_t FsmVariableInput::getId()
{
    return size_t();
}

/* OUTPUT */
size_t FsmVariableOutput::getId()
{
    return size_t();
}
