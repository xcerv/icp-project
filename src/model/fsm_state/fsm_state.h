
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
#include <unordered_set>
#include "mvc_interface.h"

class FsmState
{
    protected:
        string name;
        FsmPoint position;

        stateType type; // May be only NORMAL or FINAL; ACTIVE can be tracked globally with state_id, since it's always one state

        // Reference to the map
        unordered_set<size_t> transitions;
        vector<size_t> actions; 

    public:
        FsmState(string n, FsmPoint p, stateType t);

        const string &getName() const;
        const FsmPoint &getPosition() const;
        const stateType &getType() const;

        const size_t &getActionByOrder(size_t order) const;

        const vector<size_t> &getActions();
        const unordered_set<size_t> &getTransitions();

        bool setName(string name);
        bool setPosition(FsmPoint position);
        bool setType(stateType type);

        bool addTransition(size_t id);
        bool removeTransition(size_t id);

        bool addAction(size_t id, size_t currOrder, size_t order);
        bool removeAction(size_t order);
};

#endif