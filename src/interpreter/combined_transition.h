/**
* Project name: ICP Project 2024/2025
*
* @file combined_transition.h
* @author  xcervia00
*
* @brief Combined transition (string + cond + timeout) used in the FSM application
*
*/

#ifndef COMBINED_TRANSITION_H
#define COMBINED_TRANSITION_H

#include <QObject>
#include <QStateMachine>
#include <QAbstractTransition>

#define REGEX_TRANSITION_CONDITION "^ *([a-zA-Z_-]+)? *(\\[(.+)\\])? *(@ *([1-9][0-9]*))? *$"

/**
 * @brief Class used for transitions in ICP FSM - it combines two possible input events (initial Input and Timeout)
 */
class CombinedTransition : public QAbstractTransition
{
    Q_OBJECT

    private:
        QString m_name; ///< Name of the input that can trigger this transition, can be empty
        QString m_guard; ///< Guard condition to check before transitioning; can be empty
        QString m_timeout; ///< Timeout before proceeding with transition; can be empty

        bool m_pending; ///< Flags whether a Timeout event spawned by this transition is pending
        int m_pending_id; ///< The id of delayed Timeout event; -1 if nothing pending 

        size_t m_id; ///< Unique identifier of the transition

    protected:
        /**
         * @brief Tests whether transition should be triggered
         * @param e Event that may trigger this transition - either Input or Timeout event
         * @return returns true if transition should be triggered, otherwise false
         */
        bool eventTest(QEvent *e) override;
        /**
         * @brief Fired when transitions is triggered; Stops all other pending transitions
         * @param e Event that triggered the transition
         */
        void onTransition(QEvent *e) override;

    public:
        /**
         * @brief Constructor for combined transition
         * @param name The of the Input event that may trigger this condition
         * @param guard The guard condition that is checked before triggering this transition
         * @param timeout The timeout to wait for once the transition is deemed triggerable
         */
        CombinedTransition(const QString &name, const QString &guard, const QString &timeout);
        /**
         * @brief Constructor that parses input string into m_name, m_guard, m_timeout
         * @param unparsed_condition The condition string to be parsed and set as the members
         */
        CombinedTransition(const QString &unparsed_condition);
        /**
         * @brief The Main constructor that sets unique id for the transition
         * @param id The unique ID to set for this transition
         */
        CombinedTransition(const size_t id);

        /**
         * @brief Setter for the conditions
         * @note Input string will be parsed into m_name, m_guard, m_timeout
         * @param condition The condition to be processed and set
         * @return Returns true on success, otherwise false
         */
        bool setCondition(const QString &condition);

        /**
         * @brief Getter for the unique identifier (m_id) of this state
         * @return Returns size_t being the unique ID of this state
         */
        size_t getId() const;
        
        /**
         * @brief Stops any delayed events started by this transition
         * @note Restarts m_pending and m_pending_id to default
         */
        void stopTimer();

};

#endif // COMBINEDTRANSITION_H
