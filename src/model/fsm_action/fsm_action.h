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
        size_t order;

    public:
        FsmAction(string act, size_t ord);

        bool execute();

        const string &getAction() const;
        const size_t &getOrder() const;

        bool setAction(string action);
        bool setOrder(size_t newOrder);
};

#endif