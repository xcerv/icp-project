/**
 * Project name: ICP Project 2024/2025
 *
 * @file state.h
 * @author  xlogin00
 *
 * @brief Interface for classes working with MVC
 *
 */

#ifndef MVC_INTERFACE_H_
#define MVC_INTERFACE_H_

using namespace std;
#include <string>

enum varType : int
{
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    CHAR,
    BOOL,
};

enum stateType : int
{
    NORMAL,
    FINAL,
    ACTIVE,
};

struct FsmPoint
{
  ssize_t x;
  ssize_t y;
};

class FsmInterface
{
    public:
        virtual void updateState(size_t id, string name, FsmPoint pos, stateType type) = 0;
        virtual void updateAction(size_t id, size_t parent_state_id, string action) = 0;
        virtual void updateCondition(size_t parent_transition_id, string condition) = 0;
        virtual void updateTransition(size_t id, size_t id_state_src, size_t id_state_dest) = 0;
        virtual void updateVarInput(size_t id, string name, string value) = 0;
        virtual void updateVarOutput(size_t id, string name, string value) = 0;
        virtual void updateVarInternal(size_t id, string name, string value, varType type) = 0;

        virtual void destroyState(size_t id) = 0;
        virtual void destroyAction(size_t id) = 0;
        virtual void destroyCondition(size_t parent_id) = 0;
        virtual void destroyTransition(size_t id) = 0;
        virtual void destroyVarInput(size_t id) = 0;
        virtual void destroyVarOutput(size_t id) = 0;
        virtual void destroyVarInternal(size_t id) = 0;

        virtual void loadFile(string filename) = 0;
        virtual void saveFile(string filename) = 0;

        // This may be used only one-way
        virtual void log(string time, string state, string varInputs, string varOutputs, string varInternals) = 0;

        virtual void cleanup() = 0;
        virtual void throwError(int errnum) = 0;
};

#endif