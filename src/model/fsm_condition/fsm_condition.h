/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_condition.h
 * @author xcervia00
 *
 * @brief Interface of Condition class
 *
 */

#ifndef FSM_CONDITION_H_
#define FSM_CONDITION_H_

class FsmCondition
{
    protected:
        inline static size_t id = 0;

        string condition;

    public:
        FsmCondition();

        size_t getId();
        string getCondition();

        bool setCondition(string cond);

        bool evaluate();
};

#endif