/**
 * Project name: ICP Project 2024/2025
 *
 * @file model_interpret.cpp
 * @author xcervia00
 *
 * @brief The interpretation portion of Model implementation
 *
 */

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "mvc_interface.h"
#include "model.h"

using namespace std;

void FsmModel::renameFsm(const QString &name)
{
    this->machine.setObjectName(name);
    view->renameFsm(name);
}

void FsmModel::startInterpretation()
{
    return; // Todo
}

void FsmModel::stopInterpretation()
{
    return; // Todo
}