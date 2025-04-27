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
#define REGEX_VARIABLE
// Internal variable regex ==> type and 
#define REGEX_VARIABLE_INTERNAL "" REGEX_VARIABLE ""
#define REGEX_VARIABLE_INPUT REGEX_VARIABLE
#define REGEX_VARIABLE_OUTPUT REGEX_VARIABLE

// Regex for states
#define REGEX_STATE

// Regex for transitions
#define REGEX_TRANSITION

/**
 * @brief 
 * @param filename 
 */
void FsmModel::loadFile(const QString &filename)
{

}

/**
 * @brief 
 * @param filename 
 */
void FsmModel::saveFile(const QString &filename)
{

}
