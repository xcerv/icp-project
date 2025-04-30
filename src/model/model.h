/**
 * Project name: ICP Project 2024/2025
 *
 * @file model.h
 * @author xcervia00
 *
 * @brief Interface for Model class
 *
 */

#ifndef FSM_MODEL_H_
#define FSM_MODEL_H_

#include "mvc_interface.h"

// #include "fsm_state/fsm_state.h"
// #include "fsm_condition/fsm_condition.h"
// #include "fsm_transition/fsm_transition.h"
// #include "fsm_action/fsm_action.h"
// #include "fsm_variable/fsm_variable.h"

#include "action_state.h"
#include "combined_transition.h"
#include "script_helper.h"
#include "fsm_exceptions.h"

#include <QJSEngine>
#include <QStateMachine>
#include <QHash>
#include <QObject>
#include <QVariant>

using namespace std;

#define CATCH_MODEL(code)                                                           \
                            try{                                                    \
                                code                                                \
                            }                                                       \
                            catch(const FsmModelException &ex){                     \
                                this->view->throwError(ex.errorCode(), ex.what());  \
                                return;                                             \
                            }                                                        


struct ContextBackup
{
    QHash<QString,QVariant> vInternal;
    QHash<QString,QString> vInput;
    QHash<QString,QString> vOutput;
    QAbstractState* initialState; 
};

/* Classes */
class FsmModel : public FsmInterface
{
    friend class ScriptHelper;

    protected:
        QJSEngine engine; ///< Native javascript interpreter for evaluating conditions/actions
        QStateMachine machine; ///< Main FSM machine to be interpreted

        FsmInterface* view = nullptr; ///< Reference to view

        QHash<QString,ActionState*> states; ///< List of all states used within the FSM
        QHash<size_t,CombinedTransition*> transitions;
        QHash<QString,QVariant> varsInternal; ///< Internal variable - may be of variable type
        QHash<QString,QString> varsInput; ///< Input variable - only string format
        QHash<QString,QString> varsOutput; ///< Output variable - only string format

        ContextBackup backup; ///< Backup of machine state prior to interpretation
        ScriptHelper scriptHelper; ///< Separate interface for communication with QJSEngine

    public:
        FsmModel();
        virtual ~FsmModel();

        // Interface methods
        void updateState(const QString &name, const QPoint &pos) override;
        void updateStateName(const QString &oldName, const QString &newName) override;
        void updateAction(const QString &parentState, const QString &action) override;
        void updateActiveState(const QString &name) override;

        void updateCondition(size_t transitionId, const QString &condition) override;
        void updateTransition(size_t transitionId, const QString &srcState, const QString &destState) override;
        void updateVarInput(const QString &name, const QString &value) override;
        void updateVarOutput(const QString &name, const QString &value) override;
        void updateVarInternal(const QString &name, const QVariant &value) override;

        void destroyState(const QString &name) override;
        void destroyAction(const QString &parentState) override;
        void destroyCondition(size_t transitionId) override;
        void destroyTransition(size_t transitionId) override;
        void destroyVarInput(const QString &name) override;
        void destroyVarOutput(const QString &name) override;
        void destroyVarInternal(const QString &name) override;

        void loadFile(const QString &filename) override;
        void saveFile(const QString &filename) override;

        void renameFsm(const QString &name) override;

        void parseVariableLine(const QString &line);
        void parseStateLine(const QString &line);
        void parseTransitionLine(const QString &line, size_t id);

        // This may be used only one-way
        void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const override;
        void log() const override; // Request log

        void startInterpretation() override;
        void stopInterpretation() override;

        void cleanup() override;
        void throwError(FsmErrorType errNum) override;
        void throwError(FsmErrorType errNum, const QString &errMsg) override;

        void outputEvent(const QString &outName) override;
        void inputEvent(const QString &name, const QString &value) override;

        // Model specific
        void registerView(FsmInterface *view);
        QAbstractState* getActiveState() const;

        // Machine getter
        QStateMachine *getMachine();

        // Interpretation error
        void interpretationError(FsmErrorType errNum);
        void interpretationError(FsmErrorType errNum, const QString &errMsg);

    /* Template getters/setters */
    private:    
        /**
         * @brief Tempate for safely getting elements out of model's internal containters
         * @tparam Key The key to search the element by
         * @tparam Value The type of value that is stored in the container
         * @param container Container storing the elements
         * @param key The key to look for in the container
         * @param errnum The error number thrown when element is not found
         * @param errMsg The specific message that may be used when printing error to user
         * @return Returns the element of type Value
         */
        template <typename Key, typename Value>
        Value safeGetter(const QHash<Key, Value> &container, const Key &key, const FsmModelException &err)
        {
            auto it = container.find(key);
        
            if (it != container.end()) {
                // Key exists, call the update
                return it.value();
            } else {
                // Key does not exist, throw error
                throw err;
            }
        }

        /**
         * @brief Safely searches model's internal containers for an element to update
         * @tparam Key The key to search the element by
         * @tparam Value The type of value that is stored in the container
         * @tparam UpdateFunc The function that is used to update the element 
         * @param container Container storing the elements
         * @param key The key to look for in the container
         * @param errnum The error number thrown when element is not found
         * @param errmsg The specific message that may be used when printing error to user
         */
        template <typename Key, typename Value, typename UpdateFunc>
        void safeUpdate(QHash<Key, Value> &container, const Key &key, UpdateFunc &&updater, const FsmModelException &err)
        {
            auto it = container.find(key);

            if (it != container.end()) {
                // Key exists, call the update
                updater(it.value());
            } else {
                // Key does not exist, throw error
                throw err;
            }
        }

};

#endif
