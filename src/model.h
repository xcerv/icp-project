/**
 * Project name: ICP Project 2024/2025 
 * 
 * @file state.h
 * @author  xlogin00
 *
 * @brief Interface for states...
 * 
 */

/**
* @brief ...
*
/
  
class FsmCondition
{
  protected: 
    inline static size_t id = 0;
  
    string condition;

  public:
    void setCondition(string cond);
    string getCondition();
    bool evaluate();
}
  
class FsmTransition
{
  protected:
    inline static size_t id = 0;

    FsmState source;
    FsmState dest;
    FsmCondition transCondition; // Condition for transition
    
  public:
    FsmTransition();

    FsmState getSource();
    FsmState getDestination();
    FsmCondition getCondition();

    bool setSource(FsmState state);
    bool setDestination(FsmState state);
    bool setCondition(FsmCondition cond);
}

class FsmAction
{
  protected:
    string action;
    inline static size_t id = 0;
public:

    bool execute();
    bool setAction(string action);
    string getAction();
}

struct FsmPoint
{
  int x;
  int y;
}

class FsmState
{
  protected:
    inline static size_t id = 0;

    string name;
    FsmPoint position;
    size_t timeout;
    vector<FsmTransition> transitions;
    vector<FsmAction> actions;
    
}

// Abstract...
class FsmVariable
{
  protected:
    string name;
    string value;
  public:
    string getName();
    void setName(string name);

    string getValue();
    void setValue(string value);
    // Getters and setters
}

class FsmVariableInternal : FsmVariable 
{
  protected:
    inline static size_t id = 0;
}

class FsmVariableInput : FsmVariable 
{
  protected:
    inline static size_t id = 0;
}

class FsmVariableOutput : FsmVariable 
{
  protected:
    inline static size_t id = 0;
}

class FsmModel
{
  protected:
    string name;

    FsmState &currentState;

    vector<FsmStates> states;
    vector<FsmAction> actions;
    vector<FsmCondition> conditions;
    vector<FsmTransition> transitions;
    vector<FsmVariableInternal> varsInternal;
    vector<FsmVariableInput> varsInput;
    vector<FsmVariableOutput> varsOutput;

public:
    void receiveState(size_t id, string name, FsmPoint pos);
    void receiveAction(size_t id, string action);
    void receiveCondition(size_t id, string condition);
    void receiveTransition(size_t id, size_t id_state_src, size_t id_state_dest);
    void receiveVarInput(size_t id, string name, string value);
    void receiveVarOutput(size_t id, string name, string value);
    void receiveVarInternal(size_t id, string name, string value);    
  // Maybe timer???
}
