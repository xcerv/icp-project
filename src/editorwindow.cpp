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
#include "statefsmwidget.h"
#include <QVBoxLayout>


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
    for (StateFSMWidget* state : allStates) {
        delete state;  // free all condition labels
    }
    allStates.clear();  // ensure the vector is empty
    delete workAreaScrollContainer;
    delete workAreaScrollLayout;
    delete workArea;
    delete statusBarLabel;
    delete ui;
}

void EditorWindow::resizeWorkArea(int width, int height){
    //TODO: check for potentional states going out of widget
    workArea->setSizeWA(width, height);
}

void EditorWindow::workAreaLeftClick(QPoint position){
    statusBarLabel->setText("left: " + QString::number(position.x()) + ", " + QString::number(position.y()));
}

void EditorWindow::workAreaRightClick(QPoint position){
    statusBarLabel->setText("right: " + QString::number(position.x()) + ", " + QString::number(position.y()));
    insertFSMState(position, "state"); //todo: get name of state from user
}


void EditorWindow::stateFSMRightClick(){
    statusBarLabel->setText("DEBUG: right-clicked on state");
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on
    stateClicked->recolor("#b3d1ff","navy");

    //debug outputs
    //stateClicked->setName("right-clicked");
    //stateClicked->addCondition("condition");
}

void EditorWindow::stateFSMLeftClick(){
    statusBarLabel->setText("DEBUG: left-clicked on state");
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on
    stateClicked->recolor("red","green");
}

void EditorWindow::insertFSMState(QPoint position, QString name){
    bool canBeInserted = true;
    QPoint sizeWA = workArea->getSizeWA();
    QPoint sizeS; sizeS.setX(150); sizeS.setY(180); //TODO: fix if states can be multiple sizes
    //check if fits into workArea
    int sx = position.x() + sizeS.x();
    int sy = position.y() + sizeS.y();
    canBeInserted = canBeInserted && sx < sizeWA.x() && sy < sizeWA.y();
    for(StateFSMWidget * state: allStates){
        QPoint fPos = state->getPosition();
        QPoint fSize = state->getSize();
        int fsx = fPos.x() + fSize.x();
        int fsy = fPos.y() + fSize.y();
        canBeInserted = canBeInserted && !( //TODO: fix if states can be diffrent sizes
                            (fPos.x() < position.x() && fsx > position.x() && fPos.y() < position.y() && fsy > position.y()) ||
                            (fPos.x() < sx && fsx > sx && fPos.y() < sy && fsy > sy) ||
                            (fPos.x() < position.x() && fsx > position.x() && fPos.y() < sy && fsy > sy) ||
                            (fPos.x() < sx && fsx > sx && fPos.y() < position.y() && fsy > position.y())
                                           );
    }
    if(canBeInserted){
        StateFSMWidget * s = new StateFSMWidget(position,this);
        s->setParent(workArea);
        s->move(position);
        s->setName(name);
        s->show();
        connect(s, &StateFSMWidget::rightClick, this, &EditorWindow::stateFSMRightClick);
        connect(s, &StateFSMWidget::leftClick, this, &EditorWindow::stateFSMLeftClick);
        allStates.push_back(s);
    }else{ //
        statusBarLabel->setText("DEBUG: state didnt fit");
    }
}

