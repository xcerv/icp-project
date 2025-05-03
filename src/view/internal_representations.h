/**
 * Project name: ICP Project 2024/2025
 *
 * @file internal_repressentations.h
 * @author  xkadlet00
 *
 * @brief Helper header for enumerations, etc.
 *
 */

#ifndef INTERNAL_REPRESENTATIONS_H
#define INTERNAL_REPRESENTATIONS_H

#include<QLabel>

/**
 * @brief enumeration of possible types of variables in FSM
 */
enum variableType{
    INPUTV,
    OUTPUTV,
    INTERNALV,
    NUMV // Count of all variable types
};

/**
 * @brief Represenation for variables and their values
 */
struct FSMVariable {
    QLabel *name;///< name of variable
    QLabel *value;///< value of variable
};

#endif // INTERNAL_REPRESENTATIONS_H
