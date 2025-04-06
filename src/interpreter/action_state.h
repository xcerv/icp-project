/**
* Project name: ICP Project 2024/2025
*
* @file action_state.h
* @author  xcervia00
*
* @brief Extension of QState to allow for storage and execution of Actions
*
*/

#ifndef ACTION_STATE_
#define ACTION_STATE_

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QPoint>
#include <QJSEngine>

class ActionState : public QState
{
    Q_OBJECT

    protected:
        QString m_action;
        QPoint m_position;

        void onEntry(QEvent *event) override;

    public:
        ActionState(const QString &action, const QPoint &position);

        void executeAction();

        /**
         * @brief Returns pointer to the ScriptEngine
         * @return Returns pointer to ScriptEngine
         */
        QJSEngine *m_scriptEngine() const;

        bool setPosition(const QPoint &position);
        const QPoint &getPosition() const;

        bool setAction(const QString &action);
        const QString &getAction() const;
};


#endif