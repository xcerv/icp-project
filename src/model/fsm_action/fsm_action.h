/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_action.h
 * @author xcervia00
 *
 * @brief Interface of Action class
 *
 */

#ifndef FSM_ACTION_H_
#define FSM_ACTION_H_

class FsmAction
{
    protected:
        string action;
        inline static size_t id = 0;
    public:
        FsmAction();

        bool execute();
        size_t getId();
        string getAction();

        bool setAction(string action);
};

#endif