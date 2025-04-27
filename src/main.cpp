/**
 * Project name: ICP Project 2024/2025
 *
 * @file main.cpp
 * @author  xkadlet00
 *
 * @brief initiates whole program
 *
 */

#include "editorwindow.h"
#include "model.h"

#include <QApplication>


#include "debug_model/QMachineDebug.h"

int main(int argc, char *argv[])
{
    // QApplication (must be first)
    QApplication a(argc, argv);

    EditorWindow w; // Create view/controller
    FsmModel m; // Create model

    // register references
    w.registerModel(&m);
    m.registerView(&w);
    
    w.show();
    return a.exec();
}
