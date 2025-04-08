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

#include <QJSEngine>
#include <QStateMachine>
#include <QHash>
#include <QObject>
#include <QVariant>


using namespace std;

/* Forward Declare */

#define ID_UNSET SIZE_MAX

/* Classes */
class FsmModel : public FsmInterface
{

    protected:
        QStateMachine machine; ///< Main FSM machine to be interpreted
        QJSEngine engine; ///< Native javascript interpreter for evaluating conditions/actions

        shared_ptr<FsmInterface> view; ///< Reference to view

        QHash<QString,ActionState*> states; ///< List of all states used within the FSM
        QHash<size_t,CombinedTransition*> transitions;
        QHash<QString,QVariant> varsInternal; ///< Internal variable - may be of variable type
        QHash<QString,QString> varsInput; ///< Input variable - only string format
        QHash<QString,QString> varsOutput; ///< Output variable - only string format

    public:
        FsmModel();

        // Interface methods
        void updateState(const QString &name, const QPoint &pos) override;
        void updateAction(const QString &parentState, const QString &action) override;
        void updateInitialState(const QString &name) override;

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

        // This may be used only one-way
        void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const override;
        virtual void log() const = 0; // Request log

        void startInterpretation() override;
        void stopInterpretation() override;

        void cleanup() override;
        void throwError(int errnum) override;

        // Model specific
        void registerView(shared_ptr<FsmInterface> view);
};

#endif
