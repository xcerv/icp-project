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

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorWindow w;
    w.show();
    return a.exec();
}
