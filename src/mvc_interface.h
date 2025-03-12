/**
 * Project name: ICP Project 2024/2025
 *
 * @file state.h
 * @author  xlogin00
 *
 * @brief Interface for states...
 *
 */

#ifndef MVC_INTERFACE_H_
#define MVC_INTERFACE_H_

using namespace std;
#include <string>

struct FsmPoint
{
  ssize_t x;
  ssize_t y;
};

class FsmInterface
{
    public:

        virtual void updateState(size_t id, string name, FsmPoint pos) = 0;
        virtual void updateAction(size_t id, string action) = 0;
        virtual void updateCondition(size_t id, string condition) = 0;
        virtual void updateTransition(size_t id, size_t id_state_src, size_t id_state_dest) = 0;
        virtual void updateVarInput(size_t id, string name, string value) = 0;
        virtual void updateVarOutput(size_t id, string name, string value) = 0;
        virtual void updateVarInternal(size_t id, string name, string value) = 0;

        virtual void loadFile(string filename) = 0;
        virtual void saveFile(string filename) = 0;

        virtual void saveFile(string filename) = 0;

        virtual void cleanup() = 0;
        virtual void throwError(int errnum) = 0;
};

#endif