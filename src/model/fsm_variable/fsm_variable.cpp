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

FsmVariable::FsmVariable(string name, string value):
    name{name},
    value{value}
{

}

const string &FsmVariable::getName() const
{
    return this->name;
}

const string &FsmVariable::getValue() const
{
    return this->value;
}

bool FsmVariable::setName(string name)
{
    this->name = name;
    return true;
}

bool FsmVariable::setValue(string value)
{
    this->value = value;
    return true;
}

/* GENERIC */


/* INTERNAL */
FsmVariableInternal::FsmVariableInternal(string name, string value, varType type) : FsmVariable(name, value),
    type{type}
{

}

const varType FsmVariableInternal::getType() const
{
    return this->type;
}

bool FsmVariableInternal::setType(varType newType)
{
    this->type = newType;
    return true;
}

/* INPUT */
FsmVariableInput::FsmVariableInput(string name, string value) : FsmVariable(name, value)
{
}

/* OUTPUT */
FsmVariableOutput::FsmVariableOutput(string name, string value) : FsmVariable(name, value)
{
}
