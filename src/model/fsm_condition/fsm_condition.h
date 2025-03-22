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
        string condition;

    public:
        FsmCondition(string cond);

        const string &getCondition() const;

        bool setCondition(string cond);

        bool evaluate();
};

#endif