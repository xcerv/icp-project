/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_variable.h
 * @author xcervia00
 *
 * @brief Interface of Variable classes
 *
 */

#ifndef FSM_VARIABLE_H_
#define FSM_VARIABLE_H_

#include "mvc_interface.h"

/**
 * @brief Abstract class for representing variables
 */
class FsmVariable
{
    protected:
        string name; ///< Name of the 
        string value;

    public:
        FsmVariable(string name, string value);

        virtual const string &getName() const;
        virtual const string &getValue() const;

        virtual bool setName(string name);
        virtual bool setValue(string value);
};

/**
 * @brief Internal variable class
 */
class FsmVariableInternal : public FsmVariable
{
    protected:
        varType type;
    public:
        FsmVariableInternal(string name, string value, varType type);

        const varType getType() const;
        bool setType(varType newType);
};

/**
 * @brief Input variable class
 */
class FsmVariableInput : public FsmVariable
{
    public:
        FsmVariableInput(string name, string value);
};


/**
 * @brief Output variable class 
 */
class FsmVariableOutput : public FsmVariable
{
    public:
        FsmVariableOutput(string name, string value);
};

#endif