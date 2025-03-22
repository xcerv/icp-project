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

        virtual size_t getId();
        virtual string getName();
        virtual string getValue();

        virtual bool setName(string name);
        virtual bool setValue(string value);
};


/**
 * @brief Internal variable class
 */
class FsmVariableInternal : FsmVariable
{
    protected:
        varType type;
        inline static size_t id = 0;
    public:
        size_t getId();
};

/**
 * @brief Input variable class
 */
class FsmVariableInput : FsmVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};


/**
 * @brief Output variable class 
 */
class FsmVariableOutput : FsmVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

#endif