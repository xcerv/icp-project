
/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_state.h
 * @author xcervia00
 *
 * @brief 
 *
 */

#ifndef FSM_STATE_H_
#define FSM_STATE_H_

#include <string>
#include <vector>
#include "mvc_interface.h"

class FsmState
{
    protected:
        inline static size_t id = 0;

        string name;
        FsmPoint position;

        stateType type; // May be only NORMAL or FINAL; ACTIVE can be tracked globally with state_id, since it's always one state

        // Reference to the map
        vector<size_t> transitions;
        vector<size_t> actions; 

    public:
        //FsmState();

        size_t getId();
        string getName();
        FsmPoint getPosition();

        bool setName(string name);
        bool setPosition(FsmPoint point);

        bool addTransition(size_t id);
        bool removeTransition(size_t id);

        bool addAction(size_t id);
        bool removeAction(size_t id);
};

#endif