/**
 * Project name: ICP Project 2024/2025
 *
 * @file mvc_interface.h
 * @author  xcervia00
 *
 * @brief Interface for classes working with MVC
 *
 */

#ifndef MVC_INTERFACE_H_
#define MVC_INTERFACE_H_

#include <QObject>
#include <QString>

namespace FsmFormats
{
    constexpr const char *FORMAT_STATE = R"([a-zA-Z0-9_-]{1,256})";
    constexpr const char *FORMAT_VARIABLE = R"([a-zA-Z0-9_-]{1,256})";
    constexpr const char *FORMAT_ACTION = R"(.+)";
    constexpr const char *FORMAT_VARIABLE_TYPE = R"((int|float|bool|string))";
}

/**
 * @brief Enumeration of error types
 */
enum FsmErrorType : int
{
    ERROR_GENERIC,

    // Model specific, probably
    ERROR_UNDEFINED_STATE,
    ERROR_UNDEFINED_TRANSITION,
    ERROR_CONDITION_FORMAT,

    ERROR_INVALID_NAMING_FORMAT,

    ERROR_RENAME_EXISTING,

    // Interpreter specific, probably
    ERROR_INTERPRETATION_EVALUATION,

    ERROR_FILE_INVALID_FORMAT,    
};

/**
 * @brief Structure for passing error type and its message by pointer
 */
struct FsmError
{
    FsmErrorType errNum = ERROR_GENERIC;
    const char *errMsg = "";
};

/**
 * @brief Deprecated enum for variable types
 */
enum varType : int
{

};

/**
 * @brief Enumeration for type of a state
 */
enum stateType : int
{
    NORMAL,
    FINAL,
    ACTIVE,
};

/**
 * @brief The shared interface used for communication between model/view/controller
 */
class FsmInterface
{
    public:
        /**
         * @brief Creates or Updates existing state; only registers it by name and position
         * @param name The name of the new state to create or update
         * @param pos The position of the new state created; or the new position of existing state
         */
        virtual void updateState(const QString &name, const QPoint &pos) = 0;

        /**
         * @brief Renames an existing state
         * @param oldName Original name of the state
         * @param newName A new name to rename the state to
         */
        virtual void updateStateName(const QString &oldName, const QString &newName) = 0;

        /**
         * @brief Updates the action (output) of an existing state
         * @param parentState The state whose action is to be updated
         * @param action A string containing code in inscription language (JS), 
         * describing which actions are to be performed when state is entered 
         */
        virtual void updateAction(const QString &parentState, const QString &action) = 0;
        
        /**
         * @brief Makes given state active (e.g. highlights color)
         * @param name The name of the state to be made active
         */
        virtual void updateActiveState(const QString &name) = 0;

        /**
         * @brief Create or Update a transition between two states; The transition is identified by *unique* id
         * @param transitionId The unique transition id
         * @param srcState The source state where the transition begins
         * @param destState The destination state of the transition
         */
        virtual void updateTransition(size_t transitionId, const QString &srcState, const QString &destState) = 0;

        /**
         * @brief Update the condition of an transition
         * @param transitionId The unique id of a transition to update
         * @param condition The updated condition the transition uses
         * @note The format is: INPUT [ JS CODE ] @TIMEOUT
         */
        virtual void updateCondition(size_t transitionId, const QString &condition) = 0;
        
        /**
         * @brief Create or Update an input variable
         * @param name The name of the variable
         * @param value A string value of the variable
         */
        virtual void updateVarInput(const QString &name, const QString &value) = 0;
        /**
         * @brief Create or Update an output variable
         * @param name The name of the variable
         * @param value A string value of the variable
         */
        virtual void updateVarOutput(const QString &name, const QString &value) = 0;
        /**
         * @brief Create or Update an internal variable
         * @param name The name of the variable
         * @param value A QVariant value of the variable (the type is determined by the QVariant)
         */
        virtual void updateVarInternal(const QString &name, const QVariant &value) = 0;

        /**
         * @brief Destroys an existing state
         * @param name The name of the state to destroy
         */
        virtual void destroyState(const QString &name) = 0;
        /**
         * @brief Deletes the contents of an action within a state
         * @param parentState The state whose action is targeted
         */
        virtual void destroyAction(const QString &parentState) = 0;
        /**
         * @brief Deletes the contents of a condition within a transition
         * @param transitionId The unique ID of a transition
         */
        virtual void destroyCondition(size_t transitionId) = 0;
        /**
         * @brief Deletes an existing transition
         * @param transitionId The unique ID of a transition
         */
        virtual void destroyTransition(size_t transitionId) = 0;
        /**
         * @brief Removes an existing input variable
         * @param name Name of the variable
         */
        virtual void destroyVarInput(const QString &name) = 0;
        /**
         * @brief Removes an existing output variable
         * @param name Name of the variable
         */
        virtual void destroyVarOutput(const QString &name) = 0;
        /**
         * @brief Removes an existing internal variable
         * @param name Name of the variable
         */
        virtual void destroyVarInternal(const QString &name) = 0;

        /**
         * @brief Loads a defined fsm from a file
         * @param filename The path to a filename
         */
        virtual void loadFile(const QString &filename) = 0;

        /**
         * @brief Saves a defined fsm to a file
         * @param filename The path where to save the file
         */
        virtual void saveFile(const QString &filename) = 0;

        /**
         * @brief Renames the entire FSM
         * @param name The new name to use
         */
        virtual void renameFsm(const QString &name) = 0;

        /**
         * @brief Requests an entity to print out a log with specific values
         * @param time The current time
         * @param state The active state involved
         * @param varInputs The input variables
         * @param varOutputs The output variables
         * @param varInternals The internal variables
         */
        virtual void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const = 0;
        
        /**
         * @brief Requests logging from given entity (the log is constructed by that entity)
         */
        virtual void log() const = 0;

        /**
         * @brief Begins interpretation 
         * @note locks editing, allows the fsm to be stopped (greyed out button)
         */
        virtual void startInterpretation() = 0;

        /**
         * @brief Stops interpretation, if it is running
         * @note By default returns to the original state before interpretation
         */
        virtual void stopInterpretation() = 0;

        /**
         * @brief Executes output event (print out output variable)
         * @param outName The output variable to use
         */
        virtual void outputEvent(const QString &outName) = 0;

        /**
         * @brief Executes input event (used for interpreted fsm - this may lead to transition)
         * @param name The name of the input event
         * @param value The value associated with the input
         */
        virtual void inputEvent(const QString &name, const QString &value) = 0;
  
        /**
         * @brief Cleans up and erases the currently loaded fsm
         */
        virtual void cleanup() = 0;

        /**
         * @brief Throws an error identified by a unique error number
         * @param errNum The error number to use
         */
        virtual void throwError(FsmErrorType errNum) = 0;

        /**
         * @brief Throws an error identified by a unique error number with a specific message
         * @param errNum The error number to use
         * @param errMsg A message specific to the error
         */
        virtual void throwError(FsmErrorType errNum, const QString &errMsg) = 0;
};

#endif