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
        // Explicit due to Qt restrictions
        explicit ScriptHelper(FsmModel* model, QObject *parent = nullptr); 

        void registerFunctions();

        Q_INVOKABLE QVariant getInternal(const QString& name);
        Q_INVOKABLE void setInternal(const QString& name, const QVariant& value);

        Q_INVOKABLE QString getInput(const QString& name);
        Q_INVOKABLE void setInput(const QString& name, const QString& value);

        Q_INVOKABLE QString getOutput(const QString& name);
        Q_INVOKABLE void setOutput(const QString& name, const QString& value);


        /*
        ============================
            PREDEF. JS FUNCTIONS
        ============================
        */

        Q_INVOKABLE void output(const QString &name, const QJSValue &value);
        Q_INVOKABLE QJSValue valueof(const QString &name);
        Q_INVOKABLE bool defined(const QString &name);
        Q_INVOKABLE qint64 elapsed();
        Q_INVOKABLE void engine_error(const QJSValue &errNum, const QString &errMsg);
};

#endif