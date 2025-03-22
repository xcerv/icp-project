/**
 * Project name: ICP Project 2024/2025
 *
 * @file model.cpp
 * @author xcervia00
 *
 * @brief Implementation of Model class
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"
#include "model.h"

using namespace std;

FsmModel::FsmModel():
    name{"unnamed"},
    view{nullptr},
    currentState{0}
{
    // ...
}

// Interface methods
void FsmModel::updateState(size_t id, string name, FsmPoint pos, stateType type){}
void FsmModel::updateAction(size_t id, size_t parent_state_id, string action){}
void FsmModel::updateCondition(size_t parent_transition_id, string condition){}
void FsmModel::updateTransition(size_t id, size_t id_state_src, size_t id_state_dest){}
void FsmModel::updateVarInput(size_t id, string name, string value){}
void FsmModel::updateVarOutput(size_t id, string name, string value){}
void FsmModel::updateVarInternal(size_t id, string name, string value, varType type){}

void FsmModel::destroyState(size_t id){}
void FsmModel::destroyAction(size_t id){}
void FsmModel::destroyCondition(size_t parent_id){}
void FsmModel::destroyTransition(size_t id){}
void FsmModel::destroyVarInput(size_t id){}
void FsmModel::destroyVarOutput(size_t id){}
void FsmModel::destroyVarInternal(size_t id){}

void FsmModel::log(string time, string state, string varInputs, string varOutputs, string varInternals){}

void FsmModel::cleanup(){} // Clear the class entirely
void FsmModel::throwError(int errnum){}
