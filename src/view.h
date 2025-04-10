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

#include <QHash>
#include <QObject>
#include <QVariant>

#include "mvc_interface.h"

using namespace std;

class FsmView : public FsmInterface
{

    protected:
        shared_ptr<FsmInterface> model; ///< Reference to view

        //QHash<QString,...> states; ///< List of all states used within the FSM
        //QHash<size_t,...> transitions;
        QHash<QString,QVariant> varsInternal; ///< Internal variable - may be of variable type
        QHash<QString,QString> varsInput; ///< Input variable - only string format
        QHash<QString,QString> varsOutput; ///< Output variable - only string format

    public:
        FsmView();

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
        void log() const override; // Request log

        void startInterpretation() override;
        void stopInterpretation() override;

        void cleanup() override;
        void throwError(int errnum) override;

        void outputEvent(const QString &outName) override;

        // View specific
        void registerModel(shared_ptr<FsmInterface> model);
};

#endif
