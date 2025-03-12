/**
 * Project name: ICP Project 2024/2025
 *
 * @file state.h
 * @author xlogin00
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

using namespace std;



class FsmCondition
{
    protected:
        inline static size_t id = 0;

        string condition;

    public:
        FsmCondition();

        size_t getId();
        string getCondition();

        bool setCondition(string cond);

        bool evaluate();
};

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

class FsmAction
{
    protected:
        string action;
        inline static size_t id = 0;
    public:
        FsmAction();

        bool execute();
        size_t getId();
        string getAction();

        bool setAction(string action);
};

class FsmState
{
    protected:
        inline static size_t id = 0;

        string name;
        FsmPoint position;
        size_t timeout;

        // Reference to the map
        vector<size_t> transitions;
        vector<size_t> actions;

    public:
        FsmState();

        size_t getId();
        string getName();
        FsmPoint getPosition();
        size_t getTimeout();

        bool setName(string name);
        bool setPosition(FsmPoint point);
        bool setTimeout(size_t timeout);

        bool addTransition(size_t id);
        bool removeTransition(size_t id);

        bool addAction(size_t id);
        bool removeAction(size_t id);
};

// Abstract...
class FsmVariable
{
    protected:
        string name;
        string value;
    public:

        size_t getId();
        string getName();
        string getValue();

        bool setName(string name);
        bool setValue(string value);
};

class FsmVariableInternal : FsmVariable
{
    protected:
        varType type;
        inline static size_t id = 0;
    public:
        size_t getId();
};

class FsmVariableInput : FsmVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

class FsmVariableOutput : FsmVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

class FsmModel : public FsmInterface
{
  protected:
    string name;

    shared_ptr<FsmInterface> view; // Subject to change?
    size_t currentState;


    map<size_t, FsmState> states;
    map<size_t,FsmAction> actions;
    map<size_t,FsmCondition> conditions;
    map<size_t,FsmTransition> transitions;
    map<size_t,FsmVariableInternal> varsInternal;
    map<size_t,FsmVariableInput> varsInput;
    map<size_t,FsmVariableOutput> varsOutput;

  public:
    // Interface methods
    void updateState(size_t id, string name, FsmPoint pos) override;
    void updateAction(size_t id, string action) override;
    void updateCondition(size_t id, string condition) override;
    void updateTransition(size_t id, size_t id_state_src, size_t id_state_dest) override;
    void updateVarInput(size_t id, string name, string value) override;
    void updateVarOutput(size_t id, string name, string value) override;
    void updateVarInternal(size_t id, string name, string value, varType type) override;

    void loadFile(string filename) override;
    void saveFile(string filename) override;
    
    void cleanup() override; // Clear the class entirely
    void throwError(int errnum) override;
};

#endif