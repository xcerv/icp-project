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
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);


    statusBarLabel = new QLabel("nothing");
    statusBar()->addWidget(statusBarLabel);

    variablesDisplay = new VariablesDisplay(this);
    variablesDisplay->move(5, 5); // top-left corner
    variablesDisplay->raise(); // appearing above other widgets

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
    /*
    for (StateFSMWidget* state : allStates) {
        delete state;  // free all condition labels
    }
    allStates.clear();  // ensure the vector is empty
    delete workAreaScrollContainer;
    delete workAreaScrollLayout;
    delete workArea;
    delete statusBarLabel;
    */
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

    QMenu menu(this);  // create a QMenu

    QAction* addStateAction = menu.addAction("Add new state ...");
    connect(addStateAction, &QAction::triggered, this, [=]() {
        QString name = QInputDialog::getText(this, "Name of state","Write the name of the state to be inserted state here:");
        if(name != nullptr){
            // todo: interaction with other file before...
            insertFSMState(position, name);
        }
    });
    if(!checkIfFSMFits(position)){
        addStateAction->setEnabled(false);
    }
    QAction* closeWindowAction = menu.addAction("Close program");
    connect(closeWindowAction, &QAction::triggered, this, &EditorWindow::close);

    QAction* startProjectionAction = menu.addAction("Start projection");

    QAction* resizeWorkareaAction = menu.addAction("Resize work-area ...");
    connect(resizeWorkareaAction, &QAction::triggered, this, [=](bool){resizeWorkArea();});

    menu.exec(QCursor::pos());
}

void EditorWindow::resizeWorkArea(){
    QPoint minSize = getMinWorkAreaSize();

    QPoint sizeNow = workArea->getSizeWA();

    // make dialog for getting neccassary info
    QDialog dialog(this);
    dialog.setWindowTitle("Resize work-area");

    QFormLayout form(&dialog);

    // two spinboxes (width, height)
    QSpinBox *widthInput = new QSpinBox(&dialog);
    widthInput->setRange(minSize.x(), 9999); // width range
    widthInput->setValue(sizeNow.x());       // default value

    QSpinBox *heightInput = new QSpinBox(&dialog);
    heightInput->setRange(minSize.y(), 9999); // height range
    heightInput->setValue(sizeNow.y());       // default value

    form.addRow("Width:", widthInput);
    form.addRow("Height:", heightInput);

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // execute dialog
    if (dialog.exec() == QDialog::Accepted) {
        workArea->setSizeWA(widthInput->value(),heightInput->value());
    }
}

QPoint EditorWindow::getMinWorkAreaSize(){
    QPoint ret;
    ret.setX(200);
    ret.setY(200);
    for(StateFSMWidget * state: allStates){
        QPoint statePoint = state->getPosition();
        statePoint += state->getSize();
        if(ret.x()<statePoint.x()){
            ret.setX(statePoint.x());
        }
        if(ret.y()<statePoint.y()){
            ret.setY(statePoint.y());
        }
    }
    return ret;
}

void EditorWindow::stateFSMRightClick(){
    statusBarLabel->setText("DEBUG: right-clicked on state");
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on

    //debug
    //stateClicked->recolor("#b3d1ff","navy");
    //stateClicked->addOutput("condition");
    QMenu menu(this);  // create a QMenu

    QAction* connectToAction = menu.addAction("Connect to ...");
    QAction* deleteAction = menu.addAction("Delete");
    QAction* setStartAction = menu.addAction("Set as starting");
    QAction* g = menu.addAction("Add output ...");
    menu.exec(QCursor::pos());
}

void EditorWindow::stateFSMLeftClick(){
    statusBarLabel->setText("DEBUG: left-clicked on state");
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on

    //debug
    //stateClicked->recolor("pink","teal");
    //stateClicked->setName("right-clicked");
}

bool EditorWindow::checkIfFSMFits(QPoint position){
    bool canBeInserted = true;
    QPoint sizeWA = workArea->getSizeWA();
    QPoint sizeS; sizeS.setX(150); sizeS.setY(180); //TODO: fix if states can be multiple sizes
    //check if fits into workArea
    int sx = position.x() + sizeS.x();
    int sy = position.y() + sizeS.y();
    canBeInserted = canBeInserted && sx < sizeWA.x() && sy < sizeWA.y();
    //check for collision with other states
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
    return canBeInserted;
}

void EditorWindow::insertFSMState(QPoint position, QString name){
    StateFSMWidget * s = new StateFSMWidget(position,this);
    s->setParent(workArea);
    s->move(position);
    s->setName(name);
    s->show();
    connect(s, &StateFSMWidget::rightClick, this, &EditorWindow::stateFSMRightClick);
    connect(s, &StateFSMWidget::leftClick, this, &EditorWindow::stateFSMLeftClick);
    allStates.push_back(s);
}

void EditorWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::information(this, "Goodbye", "Program is closing");
    event->accept();
}

