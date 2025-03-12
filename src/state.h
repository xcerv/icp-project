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
  
class FsmState
{
  protected:
    string name;
    vector<FsmTransition> transitions;
    size_t timeout;
    
}

