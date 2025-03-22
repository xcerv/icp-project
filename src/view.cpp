/**
 * Project name: ICP Project 2024/2025
 *
 * @file view.cpp
 * @author xlogin00
 *
 * @brief Implementation of View class
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"
#include "view.h"

using namespace std;

// Customise accordingly
FsmView::FsmView():
    name{"unnamed"},
    model{nullptr},
    currentState{0}
{
    // ...
}

// Interface methods
void FsmView::updateState(size_t id, string name, FsmPoint pos, stateType type){}
void FsmView::updateAction(size_t id, size_t parent_state_id, string action){}
void FsmView::updateCondition(size_t parent_transition_id, string condition){}
void FsmView::updateTransition(size_t id, size_t id_state_src, size_t id_state_dest){}
void FsmView::updateVarInput(size_t id, string name, string value){}
void FsmView::updateVarOutput(size_t id, string name, string value){}
void FsmView::updateVarInternal(size_t id, string name, string value, varType type){}

void FsmView::destroyState(size_t id){}
void FsmView::destroyAction(size_t id){}
void FsmView::destroyCondition(size_t parent_id){}
void FsmView::destroyTransition(size_t id){}
void FsmView::destroyVarInput(size_t id){}
void FsmView::destroyVarOutput(size_t id){}
void FsmView::destroyVarInternal(size_t id){}

void FsmView::loadFile(string filename){}
void FsmView::saveFile(string filename){}

void FsmView::log(string time, string state, string varInputs, string varOutputs, string varInternals){}

void FsmView::cleanup(){} // Clear the class entirely
void FsmView::throwError(int errnum){}

