/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_transition.h
 * @author xcervia00
 *
 * @brief Interface of Transition class
 *
 */

#ifndef FSM_TRANSITION_H_
#define FSM_TRANSITION_H_

using namespace std;

// Forward declare
#include "model/fsm_state/fsm_state.h"
#include "model/fsm_condition/fsm_condition.h"

class FsmTransition
{
    protected:
        size_t source;
        size_t dest;
        FsmCondition transCondition; // Condition for transition

    public:
        FsmTransition(size_t src, size_t dest, string cond);

        const size_t &getSource() const;
        const size_t &getDestination() const;
        const string &getCondition() const;

        bool setSource(size_t state);
        bool setDestination(size_t state);
        bool setCondition(string condition);
};

#endif