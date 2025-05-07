/**
* Project name: ICP Project 2024/2025
*
* @file script_helper.h
* @author  xcervia00
*
* @brief An interface for accessing/setting variables from within QJSEngine
*
*/

#ifndef SCRIPT_HELPER_H
#define SCRIPT_HELPER_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QHash>
#include <QJSEngine>

class FsmModel;

/**
 * @brief Helper class working as interface between model and QJSEngine
 */
class ScriptHelper : public QObject
{
    Q_OBJECT

    private:
        FsmModel* m_model;

    public:
        /**
         * @brief Constructor for the script helper
         * @param model The parent model that holds the interpreter
         * @param parent The parent QObject (owner) - should be the QJSEngine
         */
        explicit ScriptHelper(FsmModel* model, QObject *parent = nullptr); 
        /**
         * @brief Interpreter's getter for internal variable
         * @param name Name of the variable to get
         * @return The value of the variable
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE QJSValue getInternal(const QString& name);
        /**
         * @brief Interpreter's setter for internal variables
         * @param name The name of the variable to set
         * @param value The value to set the variable to
         * @return Returns true if access was successful, otherwise false
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE bool setInternal(const QString& name, const QVariant& value);

        /**
         * @brief Interpreter's getter for Input variable
         * @param name Name of the variable to get
         * @return The value of the variable
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE QJSValue getInput(const QString& name);
        /**
         * @brief Interpreter's setter for input variables
         * @param name The name of the variable to set
         * @param value The value to set the variable to
         * @return Returns true if access was successful, otherwise false
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE bool setInput(const QString& name, const QString& value);

        /**
         * @brief Interpreter's getter for output variable
         * @param name Name of the variable to get
         * @return The value of the variable
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE QJSValue getOutput(const QString& name);
        /**
         * @brief Interpreter's setter for output variables
         * @param name The name of the variable to set
         * @param value The value to set the variable to
         * @return Returns true if access was successful, otherwise false
         * @note On invalid access stops interpretation and throws error
         */
        Q_INVOKABLE bool setOutput(const QString& name, const QString& value);


        /*
        ============================
            PREDEF. JS FUNCTIONS
        ============================
        */

        /**
         * @brief Sets an output variable and fires output event (prints the new value)
         * @param name The name of the *output* variable
         * @param value The value to set the output variable to 
         */
        Q_INVOKABLE void output(const QString &name, const QJSValue &value);
        /**
         * @brief Sets only the internal variable to given value
         * @param name The name of the variable
         * @param value The value to set the variable to
         */
        Q_INVOKABLE void set(const QString &name, const QJSValue &value);
        /**
         * @brief Get any variable's value from model's internal representaiton
         * @param name The name of the variable
         * @note priority of the variables (in case of matching names): internal->output->input 
         */
        Q_INVOKABLE QJSValue valueof(const QString &name);
        /**
         * @brief Get only the value of internal variable
         * @param name Name of the internal variable
         * @return Value of the variable
         */
        Q_INVOKABLE QJSValue get(const QString &name);
        /**
         * @brief Checks whether the given variable is defined
         * @param name Name of the variable
         * @return True if it is defined, otherwise false
         */
        Q_INVOKABLE bool defined(const QString &name);
        /**
         * @brief Returns the time since last entering the current state (only when a state was changed to another)
         * @return Time in milliseconds
         */
        Q_INVOKABLE qint64 elapsed();
        /**
         * @brief Converts given value to integer
         * @param value The value to convert
         * @return Argument convereted to integer
         */
        Q_INVOKABLE qint32 atoi(const QJSValue &value);
        /**
         * @brief Manual invocation of interpreter engine error
         * @param errNum The number representing given error
         * @param errMsg Custom error message
         * @note This should not be used normally
         */
        Q_INVOKABLE void engine_error(const QJSValue &errNum, const QString &errMsg);
        /**
         * @brief Explicitly stops interpretation
         */
        Q_INVOKABLE void stop();
};

#endif