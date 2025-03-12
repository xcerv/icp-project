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

class FsmObject
{
  int x;
  int y;
}
  
class FsmCondition
{
  protected: 
    string condition;

  public:
    void setCondition(string cond);
    string getCondition();
    bool evaluate();
}
  
class FsmTransition
{
  protected:
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
  public:

    bool execute();
    bool setAction(string action);
    string getAction();
}
  
class FsmState
{
  protected:
    string name;
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
    // Getters and setters
}

class FsmInternalVariable
{}

class FsmManager
{
  protected:
    string name;

    FsmState &currentState;
    vector<FsmStates> allStates;
    vector<FsmVariable> varsInput;
    vector<FsmVariable> vars;

    void receiveState();
    void receiveAciton();
    void receiveTransition();
    void receiveVarInput();
    void receiveVarOutput();
    void receiveVarInternal();

    
  // Maybe timer???
}
