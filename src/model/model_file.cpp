/**
 * Project name: ICP Project 2024/2025
 *
 * @file model_file.cpp
 * @author xcervia00
 *
 * @brief Implementation of file operations (load/save) for use in Model class
 *
 */

#include <QObject>
#include <QString>
#include <QFile>
#include <QRegularExpression>
#include <QPoint>
#include <QDebug>

#include "mvc_interface.h"
#include "model.h"

using namespace std;

// Regular
#define REGEX_NAME
// Variable regexes

// Generic regex for variables ==> name and 
#define REGEX_VARIABLE QRegularExpression(R"((int|float|double|bool|string|char)\s+(\w+)\s*=\s*(.+))")
// Internal variable regex ==> type and 
#define REGEX_VARIABLE_INTERNAL "" REGEX_VARIABLE ""
#define REGEX_VARIABLE_INPUT REGEX_VARIABLE
#define REGEX_VARIABLE_OUTPUT REGEX_VARIABLE

// Regex for states
#define REGEX_STATE QRegularExpression(R"((\w+)\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*:\s*\{\s*(.*)\s*\})")

// Regex for transitions
#define REGEX_TRANSITION QRegularExpression(R"((\w+)\s*->\s*(\w+)\s*:\s*\{\s*(.*)\s*\})")


void FsmModel::parseVariableLine(const QString &line) {

    auto match = REGEX_VARIABLE.match(line);
    if (!match.hasMatch()) return;

    QString type = match.captured(1);
    QString name = match.captured(2);
    QString value = match.captured(3);

    if (type == "int") {
        updateVarInternal(name, QVariant(value.toInt()));
    } 
    else if (type == "float") {
        updateVarInternal(name, QVariant(value.toFloat()));
    } 
    else if (type == "double") {
        updateVarInternal(name, QVariant(value.toDouble()));
    } 
    else if (type == "bool") {
        updateVarInternal(name, QVariant(value == "true"));
    } 
    else if (type == "char") {
        updateVarInternal(name, QVariant(value.at(0)));
    } 
    else if (type == "string") {
        updateVarInternal(name, QVariant(value));
    }
}


void FsmModel::parseStateLine(const QString &line) {

    auto match = REGEX_STATE.match(line);
    if (!match.hasMatch()) return;

    QString name = match.captured(1);
    int x = match.captured(2).toInt();
    int y = match.captured(3).toInt();
    QString action = match.captured(4);

    updateState(name, QPoint(x, y));
    updateAction(name, action);
}


void FsmModel::parseTransitionLine(const QString &line, size_t id) {

    auto match = REGEX_TRANSITION.match(line);
    if (!match.hasMatch()) return;

    QString src = match.captured(1);
    QString dst = match.captured(2);
    QString condition = match.captured(3);

    updateTransition(id, src, dst);
    updateCondition(id, condition);
}


/**
 * @brief 
 * @param filename 
 */
void FsmModel::loadFile(const QString &filename)
{
    cleanup();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throwError(ERROR_GENERIC, "Couldn't open the file");
        return;
    }

    QTextStream in(&file);
    QString line;
    enum Section { 
        NONE, 
        NAME, 
        COMMENT, 
        INPUT, 
        OUTPUT, 
        VARIABLES, 
        STATES, 
        TRANSITIONS 
    } 
    currentSection = NONE;

    size_t transitionIdCounter = 0;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (line == "Name:") {
            currentSection = NAME;
            continue;
        } 
        else if (line == "Comment:") {
            currentSection = COMMENT;
            continue;
        } 
        else if (line == "Input:") {
            currentSection = INPUT;
            continue;
        } 
        else if (line == "Output:") {
            currentSection = OUTPUT;
            continue;
        } 
        else if (line == "Variables:") {
            currentSection = VARIABLES;
            continue;
        } 
        else if (line == "States:") {
            currentSection = STATES;
            continue;
        } 
        else if (line == "Transitions:") {
            currentSection = TRANSITIONS;
            continue;
        }

        switch (currentSection) {
            case NAME:
                renameFsm(line);
                break;

            case INPUT:
                updateVarInput(line, "0");
                break;

            case OUTPUT:
                updateVarOutput(line, "0");
                break;

            case VARIABLES:
                parseVariableLine(line);
                break;

            case STATES:
                parseStateLine(line);
                break;

            case TRANSITIONS:
                parseTransitionLine(line, transitionIdCounter++);
                break;

            default:
                break;
        }
    }

    file.close();
}

/**
 * @brief 
 * @param filename 
 */
void FsmModel::saveFile(const QString &filename)
{

}
