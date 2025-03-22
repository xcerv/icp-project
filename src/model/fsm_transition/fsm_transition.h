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

// Forward declare
#include "model/fsm_state/fsm_state.h"
#include "model/fsm_condition/fsm_condition.h"

class FsmTransition
{
    protected:
        inline static size_t id = 0;

        FsmState source;
        FsmState dest;
        FsmCondition transCondition; // Condition for transition

    public:
        FsmTransition();

        size_t getId();
        FsmState getSource();
        FsmState getDestination();
        FsmCondition getCondition();

        bool setSource(FsmState state);
        bool setDestination(FsmState state);
        bool setCondition(FsmCondition cond);
};

#endif