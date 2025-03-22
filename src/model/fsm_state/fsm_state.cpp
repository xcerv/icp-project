/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsm_state.cpp
 * @author xcervia00
 *
 * @brief Implementation of State class
 *
 */

#include "fsm_state.h"

/**
 * @brief 
 * @return 
 */
size_t FsmState::getId()
{
    return size_t();
}

/**
 * @brief 
 * @return 
 */
string FsmState::getName()
{
    return string();
}

/**
 * @brief 
 * @return 
 */
FsmPoint FsmState::getPosition()
{
    return FsmPoint();
}

/**
 * @brief 
 * @param name 
 * @return 
 */
bool FsmState::setName(string name)
{
    return false;
}

/**
 * @brief 
 * @param point 
 * @return 
 */
bool FsmState::setPosition(FsmPoint point)
{
    return false;
}

bool FsmState::addTransition(size_t id)
{
    return false;
}

bool FsmState::removeTransition(size_t id)
{
    return false;
}

bool FsmState::addAction(size_t id)
{
    return false;
}

bool FsmState::removeAction(size_t id)
{
    return false;
}
