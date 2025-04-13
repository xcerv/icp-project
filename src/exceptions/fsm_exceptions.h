/**
* Project name: ICP Project 2024/2025
*
* @file fsm_exceptions.h
* @author  xcervia00
*
* @brief New exceptions used by project - interface
*
*/

#ifndef FSM_EXCEPTIONS_H_
#define FSM_EXCEPTIONS_H_

#include <exception>
#include <QString>
#include <string>
#include "fsm_exceptions.h"
#include "mvc_interface.h"


/**
 * @brief Special exception used by the model
 */
class FsmModelException : public std::runtime_error {
private:
    FsmErrorType m_errNum; ///< What type of error was thrown

public:
    FsmModelException(const FsmError &error);
    FsmModelException(FsmErrorType num, const char *msg);
    FsmModelException(FsmErrorType num, const std::string &msg);
    FsmModelException(FsmErrorType num, const QString &msg);

    // Getter for the error code type
    FsmErrorType errorCode() const noexcept;
};

#endif