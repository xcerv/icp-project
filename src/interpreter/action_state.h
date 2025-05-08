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
#include <QElapsedTimer>
#include <QJSEngine>

/**
 * @brief Class for representing states in ICP FSM
 */
class ActionState : public QState
{
    Q_OBJECT

    protected:
        QString m_action; ///< The actions that will be executed when the state is entered; in form of JS script
        QPoint m_position; ///< The current position of the state in editor

        static QPointer<ActionState> m_lastState; ///< last visited state
        QElapsedTimer m_timeVisited; ///< Time the state was entered without changing to any other state
        QElapsedTimer m_timeSinceEntry; ///< Time the state was entered
        
        void onEntry(QEvent *event) override; ///< Method that is executed when state is entered 

    public:
        /**
         * @brief Constructor for action state
         * @param action The default action to use by this state (usually is blank)
         * @param position The position of the state in the editor
         */
        ActionState(const QString &action, const QPoint &position);

        /**
         * @brief Executes the m_action using QJSEngine
         */
        void executeAction();

        /**
         * @brief Returns pointer to the ScriptEngine
         * @return Returns pointer to ScriptEngine
         */
        QJSEngine *m_scriptEngine() const;

        /**
         * @brief Setter for m_position member
         * @param position The new position to use
         * @return True on success, otherwise false 
         */
        bool setPosition(const QPoint &position);
        
        /**
         * @brief Getter for m_position
         * @return Returns the current position of the state
         */
        const QPoint &getPosition() const;

        /**
         * @brief Setter for m_action member
         * @param action New action string to be set
         * @return True on success, otherwise false
         */
        bool setAction(const QString &action);
        /**
         * @brief Getter for m_action
         * @return Returns the current action of the state
         */
        const QString &getAction() const;

        /**
         * @brief Setter for the last state visited
         * @param state That is visited now
         * @return Returns true on success, otherwise false
         */
        static bool setLastState(ActionState* state);
        /**
         * @brief Getter for last visited state
         * @return Returns pointer of the state, or nullptr if interpretation hadn't started yet
         */
        static ActionState* getLastState();

        /**
         * @brief Returns the amount of time spent in this state
         * @return Returns milliseconds spent in this state as qint64
         */
        qint64 getElapsed() const;

        /**
         * @brief Returns the amount of time spent in this state
         * @return Returns milliseconds spent in this state as qint64
         */
        qint64 getElapsedSinceEntry() const;
};


#endif