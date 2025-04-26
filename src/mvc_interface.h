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

enum FsmErrorType : int
{
    ERROR_GENERIC,

    // Model specific, probably
    ERROR_UNDEFINED_STATE,
    ERROR_UNDEFINED_TRANSITION,
    ERROR_CONDITION_FORMAT,

    ERROR_RENAME_EXISTING,

    // Interpreter specific, probably
    ERROR_INTERPRETATION_EVALUATION,
    
};

struct FsmError
{
    FsmErrorType errNum = ERROR_GENERIC;
    const char *errMsg = "";
};

enum varType : int
{

};

enum stateType : int
{
    NORMAL,
    FINAL,
    ACTIVE,
};

class FsmInterface
{
    public:
        virtual void updateState(const QString &name, const QPoint &pos) = 0;
        virtual void updateStateName(const QString &oldName, const QString &newName) = 0;
        virtual void updateAction(const QString &parentState, const QString &action) = 0;
        virtual void updateActiveState(const QString &name) = 0;

        virtual void updateCondition(size_t transitionId, const QString &condition) = 0;
        virtual void updateTransition(size_t transitionId, const QString &srcState, const QString &destState) = 0;
        virtual void updateVarInput(const QString &name, const QString &value) = 0;
        virtual void updateVarOutput(const QString &name, const QString &value) = 0;
        virtual void updateVarInternal(const QString &name, const QVariant &value) = 0;

        virtual void destroyState(const QString &name) = 0;
        virtual void destroyAction(const QString &parentState) = 0;
        virtual void destroyCondition(size_t transitionId) = 0;
        virtual void destroyTransition(size_t transitionId) = 0;
        virtual void destroyVarInput(const QString &name) = 0;
        virtual void destroyVarOutput(const QString &name) = 0;
        virtual void destroyVarInternal(const QString &name) = 0;

        virtual void loadFile(const QString &filename) = 0;
        virtual void saveFile(const QString &filename) = 0;

        virtual void renameFsm(const QString &name) = 0;

        // This may be used only one-way
        virtual void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const = 0;
        virtual void log() const = 0; // Request log

        virtual void startInterpretation() = 0;
        virtual void stopInterpretation() = 0;

        virtual void outputEvent(const QString &outName) = 0;

        virtual void cleanup() = 0;
        virtual void throwError(FsmErrorType errNum) = 0;
        virtual void throwError(FsmErrorType errNum, const QString &errMsg) = 0;
};

#endif