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

// Regular
#define REGEX_NAME
// Variable regexes

// Generic regex for variables ==> name and 
#define REGEX_VARIABLE R"(^\s*(int|float|bool|string)\s+(\w+)\s*=\s*(\w+)$)"
// Internal variable regex ==> type and 
#define REGEX_VARIABLE_INTERNAL "" REGEX_VARIABLE ""
#define REGEX_VARIABLE_INPUT R"(^(\w+)\s*=\s*(\w+)$)"
#define REGEX_VARIABLE_OUTPUT REGEX_VARIABLE

// Regex for states
#define REGEX_STATE R"(^\s*([A-Za-z0-9_-]+)\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*:\s*\{\s*(.*)\s*\}\s*$)"

// Regex for transitions
#define REGEX_TRANSITION R"(^\s*(\w+)\s*->\s*(\w+)\s*:\s*\{\s*(.*)\s*\}$)"


bool FsmModel::parseInOutVariableLine(const QString &line) {
 
    auto match = QRegularExpression(REGEX_VARIABLE_INPUT).match(line);
    if (!match.hasMatch()){
        return false;
    }
    QString name = match.captured(1);
    QString value = match.captured(2);
    if (value.isEmpty()) value = "0";
    updateVarInput(name, value);

    return true;
}


bool FsmModel::parseVariableLine(const QString &line) {

    auto match = QRegularExpression(REGEX_VARIABLE).match(line);
    if (!match.hasMatch()){
        return false;
    }

    // Extract variable type
    QString type = match.captured(1);
    // Extract variable name
    QString name = match.captured(2);
    // Extract variable value as string
    QString value = match.captured(3);

    bool ok;
    // Convert value to the correct type and update internal variable
    if (type == "int") {
        auto t = value.toInt(&ok, 10);
        if(!ok) 
        {
            return false;
        }
        
        updateVarInternal(name, QVariant(t));
    } 
    else if (type == "float") {
        auto t = value.toFloat(&ok);
        if(!ok) 
        {
            return false;
        }
        updateVarInternal(name, QVariant(t));
    } 
    else if (type == "bool") {
        if(value != "true" && value != "false")
        {
            return false;
        }
        updateVarInternal(name, QVariant(value == "true"));
    } 
    else if (type == "string") {
        updateVarInternal(name, QVariant(value));
    }
    else{
        return false;
    }

    return true;
}

bool FsmModel::parseStateLine(const QString &line) {

    auto match = QRegularExpression(REGEX_STATE).match(line);
    if (!match.hasMatch()) return false;

    // State name
    QString name = match.captured(1);

    bool ok;
    // X coordinate
    int x = match.captured(2).toInt(&ok, 10);
    if(!ok) return false;

    // Y coordinate
    int y = match.captured(3).toInt(&ok, 10);
    if(!ok) return false;

    // Action value
    QString action = match.captured(4);
    // Set state
    updateState(name, QPoint(x, y));
    // Set Action (if not blank)
    if(!action.isEmpty())
        updateAction(name, action);

    if (states.size() == 1) {
        updateActiveState(name);
    }

    return true;
}

bool FsmModel::parseTransitionLine(const QString &line) {

    auto match = QRegularExpression(REGEX_TRANSITION).match(line);
    if (!match.hasMatch()) return false;

    QString src = match.captured(1);
    QString dst = match.captured(2);
    QString condition = match.captured(3);

    // Source or destination don't exist
    if(!this->states.contains(src) || !this->states.contains(dst))
        return false;

    auto id = this->getUniqueTransitionId();

    // Create transition
    updateTransition(id, src, dst);
    // Update condition
    if(!condition.isEmpty())
        updateCondition(id, condition);

    return true;
}

void FsmModel::loadFile(const QString &filename)
{
    // Clear current FSM data before loading a new one
    this->cleanup();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throwError(ERROR_GENERIC, "Couldn't open the file");
        return;
    }

     // Section tracking for which part of the file is being parsed
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

    // Read the file line by line
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

         // Parse the content according to the current section
        switch (currentSection) {
            case NAME:
                renameFsm(line);
                break;

            case INPUT:
                if(parseInOutVariableLine(line) == false){
                    this->throwError(ERROR_FILE_INVALID_FORMAT, "Failed to parse variable from file");
                }
                    break;
    
            case OUTPUT:
                if(parseInOutVariableLine(line) == false){
                    this->throwError(ERROR_FILE_INVALID_FORMAT, "Failed to parse variable from file");
                }
                    break;

            case VARIABLES:
                if(parseVariableLine(line) == false){
                    this->throwError(ERROR_FILE_INVALID_FORMAT, "Failed to parse variable from file");
                }
                break;

            case STATES:
                if(parseStateLine(line) == false){
                    this->throwError(ERROR_FILE_INVALID_FORMAT, "Failed to parse state from file");
                }
                break;

            case TRANSITIONS:
                if (parseTransitionLine(line) == false){
                    this->throwError(ERROR_FILE_INVALID_FORMAT, "Failed to parse transition from file");
                }
                break;

            default:
                break;
        }
    }

    file.close();
}

void FsmModel::saveFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throwError(ERROR_GENERIC, "Couldn't open the file for writing");
        return;
    }

    QTextStream out(&file);

    // Name
    out << "Name:\n";
    out << machine.objectName() << "\n";

    // Inputs
    out << "Input:\n";
    for (auto input = varsInput.begin(); input != varsInput.end(); input++) {
        out << input.key() << (input.value().isEmpty() ? "" : QStringLiteral(" = ") + input.value()) << "\n";
    }
    out << "\n";

    // Outputs
    out << "Output:\n";
    for (auto output = varsOutput.begin(); output != varsOutput.end(); output++) {
        out << output.key() << (output.value().isEmpty() ? "" : QStringLiteral(" = ") + output.value()) << "\n";
    }
    out << "\n";

    // Internal variables
    out << "Variables:\n";
     for (auto variable = varsInternal.begin(); variable != varsInternal.end(); variable++) {
        QString type;
        const QVariant &val = variable.value();

        switch (val.type()) {
            case QVariant::Int:
                type = "int";
                break;
            case QVariant::Double:
                type = "float";
                break;
            case QVariant::Bool:
                type = "bool";
                break;
            case QVariant::String: {
                QString str = val.toString();
                type = "string";
                break;
            }
            default:
                continue;
        }
     
 
        out << type << " " << variable.key() << " = " << val.toString() << "\n";
    }
     out << "\n";
 
     // States
     out << "States:\n";
     auto activeState = getActiveState();

     //put active state first
     for (auto state = states.begin(); state != states.end(); state++) {
        if (state.value() == activeState) {
            auto position = state.value()->getPosition();
            out << state.key() << "(" << position.x() << "," << position.y() << "): {" << state.value()->getAction() << "}\n";
        }
     }

     for (auto state = states.begin(); state != states.end(); state++) {
        if (state.value() == activeState) {
            continue;
        }
        auto position = state.value()->getPosition();
        out << state.key() << "(" << position.x() << "," << position.y() << "): {" << state.value()->getAction() << "}\n";
     }
     out << "\n";

     // Transitions
     out << "Transitions:\n";
     for (auto transition = transitions.begin(); transition != transitions.end(); transition++) {
        CombinedTransition* t = transition.value();
        QString condition;
        if (!t->getName().isEmpty())
            condition += t->getName();
        if (!t->getGuard().isEmpty())
            condition += " [" + t->getGuard() + "]";
        if (!t->getTimeout().isEmpty())
            condition += " @ " + t->getTimeout();

        out << t->sourceState()->objectName() << " -> " << t->targetState()->objectName() << ": {" << condition << "}\n";
    }

    file.close();
}
 