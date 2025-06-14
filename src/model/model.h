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

#include "interpreter/action_state.h"
#include "interpreter/combined_transition.h"
#include "interpreter/script_helper.h"
#include "exceptions/fsm_exceptions.h"

#include <QJSEngine>
#include <QStateMachine>
#include <QHash>
#include <QObject>
#include <QVariant>
#include <QRegularExpression>
#include <QTextStream>

// Checks a format of given type of entity (by its QString value) and possibly throws an error
#define FORMAT_CHECK(errMsg, regex, ...) do{if (!checkAllValidFormat(FsmFormats::regex, __VA_ARGS__))           \
                                    {                                                                           \
                                        this->view->throwError(ERROR_INVALID_NAMING_FORMAT, errMsg);            \
                                        return;                                                                 \
                                    }                                                                           \
                                    }while(0)

// Checks a format of given type of entity (by its QString value) and possibly throws an exception
#define FORMAT_CHECK_EXCEPTION(errMsg, regex, ...) do{if (!checkAllValidFormat(FsmFormats::regex, __VA_ARGS__))         \
                                                    {                                                                   \
                                                        throw FsmModelException(ERROR_INVALID_NAMING_FORMAT, errMsg);   \
                                                    }                                                                   \
                                                    }while(0)

// Macro for exception handling within model
#define CATCH_MODEL(code)                                                           \
                            try{                                                    \
                                code                                                \
                            }                                                       \
                            catch(const FsmModelException &ex){                     \
                                this->view->throwError(ex.errorCode(), ex.what());  \
                                return;                                             \
                            }                                                        


/**
 * @brief Structure for holding a backup of all internal structures 
 */
struct ContextBackup
{
    QHash<QString,QVariant> vInternal; ///< Internal variable backup
    QHash<QString,QString> vInput; ///< Input variable backup
    QHash<QString,QString> vOutput; ///< Output variable backup
    QAbstractState* initialState; ///< Original initial state
};

/**
 * @brief Implementation of the Model part of the MVC pattern; handles internal representation of the FSM and interpretation
 */
class FsmModel : public QObject, public FsmInterface
{
    Q_OBJECT

    friend class ScriptHelper; ///< ScriptHelper here works as an interface for communication with the interpreter QJSEngine

    protected:
        QJSEngine engine; ///< Native javascript interpreter for evaluating conditions/actions
        QStateMachine machine; ///< Main FSM machine to be interpreted

        FsmInterface* view = nullptr; ///< Reference to view

        QHash<QString,ActionState*> states; ///< HTable of all states used within the FSM
        QHash<size_t,CombinedTransition*> transitions; ///< HTable of all the transitions identified by unique ID
        QHash<QString,QVariant> varsInternal; ///< Internal variable - may be of variable type
        QHash<QString,QString> varsInput; ///< Input variable - only string format
        QHash<QString,QString> varsOutput; ///< Output variable - only string format

        ContextBackup backup; ///< Backup of machine state prior to interpretation
        ScriptHelper scriptHelper; ///< Separate interface for communication with QJSEngine

        size_t uniqueTransId = 0; ///< Automatically generated unique id for transitions

    public:
        /**
         * @brief Constructor of the Model
         */
        FsmModel();
        /**
         * @brief Destructor of the model
         */
        virtual ~FsmModel();

        /* 
         =============================================
         =   Interface methods (see mvc_interface.h)
         =============================================
        */
       
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
        void loadStream(QTextStream &stream) override;
        void saveStream(QTextStream &stream) override;

        void renameFsm(const QString &name) override;

        void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const override;
        void log() const override;

        void startInterpretation() override;
        void stopInterpretation() override;
        void restoreInterpretationBackup() override;

        void cleanup() override;
        void throwError(FsmErrorType errNum) override;
        void throwError(FsmErrorType errNum, const QString &errMsg) override;

        void outputEvent(const QString &outName) override;
        void inputEvent(const QString &name, const QString &value) override;

        /* 
         ======================
         =   Model specific
         ======================
        */

        /**
         * @brief Loads model internal representation from given stream
         * @param in The stream from which to read
         */
        void loadFromStream(QTextStream &in);

        /**
         * @brief Saves model's state to given stream
         * @param out The stream to save to
         */
        void saveToStream(QTextStream &out);

        /**
         * @brief Returns the name of the active state
         * @return String value of the active state
         */
        const QString getActiveName() const;

        /**
         * @brief Checks whether any states are defined
         * @return True if any states exist, otherwise false
         */
        bool emptyStates() const;

        /**
         * @brief Registers view entity
         * @param view Pointer to entity supporting 
         */
        void registerView(FsmInterface *view);
        /**
         * @brief Returns pointer to the currently active state
         * @return Pointer to active state
         */
        QAbstractState* getActiveState() const;

        /**
         * @brief Generates unique id for transition
         * @return Unique numerical id
         */
        size_t getUniqueTransitionId();
        /**
         * @brief If parameter is zero, generate new id; otherwise return it unchanged
         * @param id The unique id; if it is 0 then new id is generated
         * @return Unique id or the parameter value unchanged
         */
        size_t getUniqueTransitionId(size_t id);

        // Machine getter

        /**
         * @brief QStateMachine getter
         * @return Returns the pointer to QStateMachine
         */
        QStateMachine *getMachine();

        // Interpretation error

        /**
         * @brief Error that occured during code evaluation
         * @param errNum The error number associated with the error
         */
        void interpretationError(FsmErrorType errNum);
        /**
         * @brief Error that occured during code evaluation
         * @param errNum The error number associated with the error
         * @param errMsg The custom error message
         */
        void interpretationError(FsmErrorType errNum, const QString &errMsg);

    /* Template getters/setters */
    private:    
        /**
         * @brief Tempate for safely getting elements out of model's internal containters
         * @tparam Key The key to search the element by
         * @tparam Value The type of value that is stored in the container
         * @param container Container storing the elements
         * @param key The key to look for in the container
         * @param err The error number/message thrown when element is not found
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
         * @param updater The function used for updating the value
         * @param err The error number/message thrown when element is not found
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

        /**
         * @brief Parses given line into its Input and Output variables interpretation
         * @param line The line to be parsed
         * @param type The type of the variable to parse(Output or Input)
         */
        bool parseInOutVariableLine(const QString &line, int type);
        
        /**
         * @brief Template that checks if all arguments match given regex
         * @tparam ...Args The type arguments pased
         * @param regexPattern The regex to check by
         * @param ...args The arguments passed
         * @return Returns true if all match the regex, otherwise false
         */
        template<typename... Args>
        bool checkAllValidFormat(const char* regexPattern, const Args&... args)
        {
            return (QRegularExpression(regexPattern).match(args).hasMatch() && ...);
        }
        
        /**
         * @brief Checks whether the given string is in accordance to the expected regex
         * @param str The string to check
         * @param regex The regex to check by
         * @return True on match, otherwise false
         */
        bool checkValidFormat(const QString &str, const char* regex);

        /**
         * @brief Parses given line into its variable interpretation
         * @param line The line to be parsed
         */
        bool parseVariableLine(const QString &line);
        /**
         * @brief Parses the given line into its state representation
         * @param line Input line to be parsed
         */
        bool parseStateLine(const QString &line);
        /**
         * @brief Parses given line into its transition interpretation
         * @param line The line to be parsed
         */
        bool parseTransitionLine(const QString &line);
};

#endif
