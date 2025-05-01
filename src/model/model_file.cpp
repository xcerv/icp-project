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
 
 
 /**
  * @brief Parses a line describing a variable and updates the internal model
  * @param line current line from the file
  */
 void FsmModel::parseVariableLine(const QString &line) {
 
     auto match = REGEX_VARIABLE.match(line);
     if (!match.hasMatch()){ 
         throwError(ERROR_GENERIC, "Line does not match variable format");
         return;
     }
 
     // Extract variable type (e.g., int, bool, etc.)
     QString type = match.captured(1);
     // Extract variable name
     QString name = match.captured(2);
     // Extract variable value as string
     QString value = match.captured(3);
 
     // Convert value to the correct type and update internal variable
     if (type == "int") {
         QRegularExpression re("^-?\\d+$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid int value: " + value);
             return;
         }
             updateVarInternal(name, QVariant(value.toInt()));
     } 
     else if (type == "float") {
         QRegularExpression re("^-?\\d*\\.\\d+$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid float value: " + value);
             return;
         }
         updateVarInternal(name, QVariant(value.toFloat()));
     } 
     else if (type == "double") {
         QRegularExpression re("^-?\\d*\\.\\d+$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid double value: " + value);
             return;
         }
         updateVarInternal(name, QVariant(value.toDouble()));
     } 
     else if (type == "bool") {
         QRegularExpression re("^(true|false)$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid bool value: " + value);
             return;
         }
         updateVarInternal(name, QVariant(value == "true"));
     } 
     else if (type == "char") {
         QRegularExpression re("^'.'$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid char value: " + value);
             return;
         }
         updateVarInternal(name, QVariant(value.at(0)));
     } 
     else if (type == "string") {
         QRegularExpression re("^\".*\"$");
         auto t = re.match(value);
         if (!t.hasMatch()) {
             throwError(ERROR_GENERIC, "Invalid string value: " + value);
             return;
         }
         updateVarInternal(name, QVariant(value));
     }
 }
 
 /**
  * @brief Parses a line describing a state and updates the model with its position and action
  * @param line current line from the file
  */
 void FsmModel::parseStateLine(const QString &line) {
 
     auto match = REGEX_STATE.match(line);
     if (!match.hasMatch()) { 
         throwError(ERROR_GENERIC, "Line does not match state format");
         return;
     }
 
     QString name = match.captured(1);
     int x = match.captured(2).toInt();
     int y = match.captured(3).toInt();
     QString action = match.captured(4);
 
     updateState(name, QPoint(x, y));
     updateAction(name, action);
 }
 
 /**
  * @brief Parses a line describing a transition and updates the model with its connection and condition
  * @param line current line from the file
  * @param id unique id of each transition
  */
 void FsmModel::parseTransitionLine(const QString &line, size_t id) {
 
     auto match = REGEX_TRANSITION.match(line);
     if (!match.hasMatch()) { 
         throwError(ERROR_GENERIC, "Line does not match transition format");
         return;
     }
 
     QString src = match.captured(1);
     QString dst = match.captured(2);
     QString condition = match.captured(3);
 
     updateTransition(id, src, dst);
     updateCondition(id, condition);
 }
 
 
 /**
  * @brief Loads a FSM from the given file
  * @param filename The file to load
  */
 void FsmModel::loadFile(const QString &filename)
 {
     // Clear current FSM data before loading a new one
     cleanup();
 
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
 
     // Used to assign unique IDs to transitions
     size_t transitionIdCounter = 0;
 
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
     QFile file(filename);
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         throwError(ERROR_GENERIC, "Couldn't open the file for writing");
         return;
     }
 
     QTextStream out(&file);
 
     // Name
     out << "Name:\n";
     out << machine.objectName() << "\n\n";
 
     // Inputs
     out << "Input:\n";
     for (auto input = varsInput.begin(); input != varsInput.end(); input++) {
         out << input.key() << "\n";
     }
     out << "\n";
 
     // Outputs
     out << "Output:\n";
     for (auto output = varsOutput.begin(); output != varsOutput.end(); output++) {
         out << output.key() << "\n";
     }
     out << "\n";


 }
 