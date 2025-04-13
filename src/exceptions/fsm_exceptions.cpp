/**
* Project name: ICP Project 2024/2025
*
* @file fsm_exceptions.cpp
* @author  xcervia00
*
* @brief New exceptions used by project
*
*/

#include "fsm_exceptions.h"

FsmModelException::FsmModelException(const FsmError &error)
    : std::runtime_error{error.errMsg},
    m_errNum{error.errNum}
{}

FsmModelException::FsmModelException(FsmErrorType num, const char *msg)
    : std::runtime_error(msg ? msg : ""),
    m_errNum{num}
{}

FsmModelException::FsmModelException(FsmErrorType num, const std::string &msg)
    : std::runtime_error(msg),
    m_errNum{num}
{}

FsmModelException::FsmModelException(FsmErrorType num, const QString &msg)
    : std::runtime_error(msg.toStdString()),
    m_errNum{num}
{}

FsmErrorType FsmModelException::errorCode() const noexcept
{
    return m_errNum;
}