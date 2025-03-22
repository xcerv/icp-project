/**
 * Project name: ICP Project 2024/2025
 *
 * @file view.h
 * @author xlogin00
 *
 * @brief Interface for View class
 *
 */

#ifndef FSM_VIEW_H_
#define FSM_VIEW_H_

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "mvc_interface.h"

using namespace std;

/* Forward Declare */
class ViewCondition;
class ViewTransition;
class ViewAction;
class ViewState;
class ViewVariable;
class ViewVariableInternal;
class ViewVariableInput;
class ViewVariableOutput;

class FsmView;

class ViewState
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
        ViewState();

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

class ViewCondition
{
  protected:
    inline static size_t id = 0;

    string condition;

  public:
    ViewCondition();

    size_t getId();
    string getCondition();

    bool setCondition(string cond);

    bool evaluate();
};

class ViewTransition
{
  protected:
    inline static size_t id = 0;

    ViewState source;
    ViewState dest;
    ViewCondition transCondition; // Condition for transition

  public:
    ViewTransition();

    size_t getId();
    ViewState getSource();
    ViewState getDestination();
    ViewCondition getCondition();

    bool setSource(ViewState state);
    bool setDestination(ViewState state);
    bool setCondition(ViewCondition cond);
};

class ViewAction
{
  protected:
    string action;
    inline static size_t id = 0;
public:
    ViewAction();

    bool execute();
    size_t getId();
    string getAction();

    bool setAction(string action);
};

// Abstract...
class ViewVariable
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

class ViewVariableInternal : ViewVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

class ViewVariableInput : ViewVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

class ViewVariableOutput : ViewVariable
{
    protected:
        inline static size_t id = 0;
    public:
        size_t getId();
};

class FsmView : public FsmInterface
{
  protected:
    string name;

    shared_ptr<FsmInterface> model; // Subject to change?
    size_t currentState;

    map<size_t,ViewState> states;
    map<size_t,ViewAction> actions;
    map<size_t,ViewCondition> conditions;
    map<size_t,ViewTransition> transitions;
    map<size_t,ViewVariableInternal> varsInternal;
    map<size_t,ViewVariableInput> varsInput;
    map<size_t,ViewVariableOutput> varsOutput;

  public:   
    FsmView();

    // Interface methods
    void updateState(size_t id, string name, FsmPoint pos, stateType type) override;
    void updateAction(size_t id, size_t parent_state_id, size_t order, string action) override;
    void updateCondition(size_t parent_transition_id, string condition) override;
    void updateTransition(size_t id, size_t id_state_src, size_t id_state_dest) override;
    void updateVarInput(size_t id, string name, string value) override;
    void updateVarOutput(size_t id, string name, string value) override;
    void updateVarInternal(size_t id, string name, string value, varType type) override;

    void destroyState(size_t id) override;
    void destroyAction(size_t id, size_t parent_state_id) override;
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

    void startInterpretation() override; // Do nothing for view?
    void stopInterpretation() override; // Do nothing for view?

    // View specific
    void registerModel(shared_ptr<FsmInterface> model);
};

#endif
