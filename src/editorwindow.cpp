/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow.c
 * @author  xkadlet00
 *
 * @brief window where FSM is edited
 *
 */

#include "editorwindow.h"
#include "ui_editorwindow.h"

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    resizeWorkArea(1000,500);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::resizeWorkArea(int width, int height){
    ui->workArea->setFixedSize(width,height);
}
