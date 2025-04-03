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


    statusBarLabel = new QLabel("nothing");
    statusBar()->addWidget(statusBarLabel);

    workArea = new WorkArea;
    connect(workArea, &WorkArea::leftClick, this, &EditorWindow::workAreaLeftClick);
    connect(workArea, &WorkArea::rightClick, this, &EditorWindow::workAreaRightClick);
    resizeWorkArea(1000,500);
    workAreaScrollContainer = new QWidget();
    workAreaScrollLayout = new QVBoxLayout(workAreaScrollContainer);  // layout for centering
    workAreaScrollLayout->addWidget(workArea);
    workAreaScrollLayout->setAlignment(Qt::AlignCenter);  // center workArea
    workAreaScrollContainer->setLayout(workAreaScrollLayout);
    ui->workAreaScroll->setWidget(workAreaScrollContainer);  // add container to scroll area
}

EditorWindow::~EditorWindow()
{
    delete workAreaScrollContainer;
    delete workAreaScrollLayout;
    delete workArea;
    delete statusBarLabel;
    delete ui;
}

void EditorWindow::resizeWorkArea(int width, int height){
    workArea->setFixedSize(width,height);
}

void EditorWindow::workAreaLeftClick(QPoint position){
    statusBarLabel->setText("left: " + QString::number(position.x()) + ", " + QString::number(position.y()));
}

void EditorWindow::workAreaRightClick(QPoint position){
    statusBarLabel->setText("right: " + QString::number(position.x()) + ", " + QString::number(position.y()));
}

void EditorWindow::displayCoords(){

}

