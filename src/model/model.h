/**
 * Project name: ICP Project 2024/2025
 *
 * @file model.h
 * @author xcervia00
 *
 * @brief Interface for Model class
 *
 */

#ifndef FSM_MODEL_H_
#define FSM_MODEL_H_

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"

#include "fsm_state/fsm_state.h"
#include "fsm_condition/fsm_condition.h"
#include "fsm_transition/fsm_transition.h"
#include "fsm_action/fsm_action.h"
#include "fsm_variable/fsm_variable.h"

using namespace std;

/* Forward Declare */

/* Classes */
class FsmModel : public FsmInterface
{
  protected:
    string name;

    shared_ptr<FsmInterface> view; // Subject to change?
    size_t currentState;

    map<size_t,FsmState> states;
    map<size_t,FsmAction> actions;
    map<size_t,FsmCondition> conditions;
    map<size_t,FsmTransition> transitions;
    map<size_t,FsmVariableInternal> varsInternal;
    map<size_t,FsmVariableInput> varsInput;
    map<size_t,FsmVariableOutput> varsOutput;

  public:
    FsmModel();

    // Interface methods
    void updateState(size_t id, string name, FsmPoint pos, stateType type) override;
    void updateAction(size_t id, size_t parent_state_id, string action) override;
    void updateCondition(size_t parent_transition_id, string condition) override;
    void updateTransition(size_t id, size_t id_state_src, size_t id_state_dest) override;
    void updateVarInput(size_t id, string name, string value) override;
    void updateVarOutput(size_t id, string name, string value) override;
    void updateVarInternal(size_t id, string name, string value, varType type) override;

    void destroyState(size_t id) override;
    void destroyAction(size_t id) override;
    void destroyCondition(size_t parent_id) override;
    void destroyTransition(size_t id) override;
    void destroyVarInput(size_t id) override;
    void destroyVarOutput(size_t id) override;
    void destroyVarInternal(size_t id) override;

    void loadFile(string filename) override;
    void saveFile(string filename) override;

    void log(string time, string state, string varInputs, string varOutputs, string varInternals) override;
    
    void cleanup() override; // Clear the class entirely
    void throwError(int errnum) override;
};

#endif
