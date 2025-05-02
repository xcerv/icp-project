/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow.c
 * @author  xkadlet00
 *
 * @brief window where FSM is edited
 *
 */

#include "view/editorwindow.h"
#include "ui_editorwindow.h"
#include "view/state_fsm_widget/statefsmwidget.h"
#include "view/logging_window/loggingwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QPointer>
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    statusBarLabel = new QLabel("nothing");
    statusBar()->addWidget(statusBarLabel);

    // Variable display
    variablesDisplay = new VariablesDisplay(this);
    variablesDisplay->move(5, 5); // top-left corner
    variablesDisplay->raise(); // appearing above other widgets

    connect(variablesDisplay, &VariablesDisplay::addVariableToDisplay, this, &EditorWindow::variableToBeAdded);
    connect(variablesDisplay, &VariablesDisplay::removeVariableFromDisplay, this, &EditorWindow::variableToBeDeleted);
    connect(variablesDisplay, &VariablesDisplay::editVariableInDisplay, this, &EditorWindow::variableToBeEdited);

    // WorkAreaContrainer
    workAreaScrollContainer = new QWidget();
    workAreaScrollLayout = new QVBoxLayout(workAreaScrollContainer);  // layout for centering
    workAreaScrollContainer->setLayout(workAreaScrollLayout);
    ui->workAreaScroll->setWidget(workAreaScrollContainer);  // add container to scroll area
    ui->workAreaScroll->setWidgetResizable(true);

    // Workarea
    workArea = new WorkArea;
    connect(workArea, &WorkArea::leftClick, this, &EditorWindow::workAreaLeftClick);
    connect(workArea, &WorkArea::rightClick, this, &EditorWindow::workAreaRightClick);
    resizeWorkArea(1000,500);
    workAreaScrollLayout->addWidget(workArea);
    workAreaScrollLayout->setAlignment(workArea, Qt::AlignHCenter);
    
    // Logging Window
    loggingWindow = new LoggingWindow(this);
    loggingWindow->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    loggingWindow->setMinimumHeight(150);
    ui->verticalLayout->addWidget(loggingWindow);
    ui->verticalLayout->setStretch(0, 4);
    ui->verticalLayout->setStretch(1,1);
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


void EditorWindow::variableToBeEdited(enum variableType type){
    // make dialog for getting neccassary info
    QDialog dialog(this);
    dialog.setWindowTitle("Edit variable");

    QFormLayout form(&dialog);

    QComboBox *nameInput = new QComboBox(&dialog);
    QList<QString> keys = allVars[type].keys();
    nameInput->addItem("...");
    for (const QString &key : keys) {
        nameInput->addItem(key);
    }
    form.addRow("Edit:", nameInput);

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(type == INPUTV){
        QLineEdit *valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->currentText() != "..." && valueInput->text() != "") {
            model->updateVarInput(nameInput->currentText(),valueInput->text());
        }
    }else{
        // type selection
        QComboBox *typeSelector = new QComboBox(&dialog);
        typeSelector->addItem("String");
        typeSelector->addItem("Bool");
        typeSelector->addItem("Int");
        typeSelector->addItem("Float");
        form.addRow("Type:", typeSelector);

        QPointer<QWidget> valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);

        // dynamic replacement
        connect(typeSelector, &QComboBox::currentTextChanged, [&form, &valueInput, &dialog](const QString &text){
            QWidget *newInput = nullptr;

            if (text == "Bool") {
                QComboBox *boolCombo = new QComboBox(&dialog);
                boolCombo->addItem("True");
                boolCombo->addItem("False");
                newInput = boolCombo;
            } else if (text == "Int") {
                QSpinBox *spin = new QSpinBox(&dialog);
                spin->setMinimum(INT_MIN);
                spin->setMaximum(INT_MAX);
                newInput = spin;
            } else if (text == "Float") {
                QDoubleSpinBox *dspin = new QDoubleSpinBox(&dialog);
                dspin->setMinimum(-9999999);
                dspin->setMaximum(9999999);
                dspin->setDecimals(4);
                newInput = dspin;
            } else {
                QLineEdit *line = new QLineEdit(&dialog);
                newInput = line;
            }

            if (newInput) {
                if (valueInput) {
                    form.removeRow(2);  // careful: remove correct row
                    delete valueInput;
                }
                //add waiting here?
                valueInput = newInput;
                form.insertRow(2, "Value:", valueInput);
            }
        });
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->currentText() != "...") {
            QString type = typeSelector->currentText();

            QVariant value;

            if (type == "Bool") {
                QComboBox *boolCombo = qobject_cast<QComboBox*>(valueInput);
                value = (boolCombo->currentText() == "True");
            } else if (type == "Int") {
                QSpinBox *spin = qobject_cast<QSpinBox*>(valueInput);
                value = spin->value();
            } else if (type == "Float") {
                QDoubleSpinBox *dspin = qobject_cast<QDoubleSpinBox*>(valueInput);
                value = dspin->value();
            } else if (type == "String") {
                QLineEdit *line = qobject_cast<QLineEdit*>(valueInput);
                value = line->text();
            }
            model->updateVarInternal(nameInput->currentText(),value);
        }
    }
}

void EditorWindow::variableToBeAdded(enum variableType type){
    // make dialog for getting neccassary info
    QDialog dialog(this);
    dialog.setWindowTitle("Add variable");

    QFormLayout form(&dialog);

    QLineEdit *nameInput = new QLineEdit(&dialog);
    form.addRow("Name:", nameInput);

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(type == OUTPUTV){
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "") {
            model->updateVarOutput(nameInput->text(),"");
        }
    }else if(type == INPUTV){
        QLineEdit *valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "" && valueInput->text() != "") {
            model->updateVarInput(nameInput->text(),valueInput->text());
        }
    }else{
        // type selection
        QComboBox *typeSelector = new QComboBox(&dialog);
        typeSelector->addItem("String");
        typeSelector->addItem("Bool");
        typeSelector->addItem("Int");
        typeSelector->addItem("Float");
        form.addRow("Type:", typeSelector);

        QPointer<QWidget> valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);

        // dynamic replacement
        connect(typeSelector, &QComboBox::currentTextChanged, [&form, &valueInput, &dialog](const QString &text){
            QWidget *newInput = nullptr;

            if (text == "Bool") {
                QComboBox *boolCombo = new QComboBox(&dialog);
                boolCombo->addItem("True");
                boolCombo->addItem("False");
                newInput = boolCombo;
            } else if (text == "Int") {
                QSpinBox *spin = new QSpinBox(&dialog);
                spin->setMinimum(INT_MIN);
                spin->setMaximum(INT_MAX);
                newInput = spin;
            } else if (text == "Float") {
                QDoubleSpinBox *dspin = new QDoubleSpinBox(&dialog);
                dspin->setMinimum(-9999999);
                dspin->setMaximum(9999999);
                dspin->setDecimals(4);
                newInput = dspin;
            } else {
                QLineEdit *line = new QLineEdit(&dialog);
                newInput = line;
            }

            if (newInput) {
                if (valueInput) {
                    form.removeRow(2);  // careful: remove correct row
                    delete valueInput;
                }
                //add waiting here?
                valueInput = newInput;
                form.insertRow(2, "Value:", valueInput);
            }
        });
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "") {
            QString type = typeSelector->currentText();

            QVariant value;

            if (type == "Bool") {
                QComboBox *boolCombo = qobject_cast<QComboBox*>(valueInput);
                value = (boolCombo->currentText() == "True");
            } else if (type == "Int") {
                QSpinBox *spin = qobject_cast<QSpinBox*>(valueInput);
                value = spin->value();
            } else if (type == "Float") {
                QDoubleSpinBox *dspin = qobject_cast<QDoubleSpinBox*>(valueInput);
                value = dspin->value();
            } else if (type == "String") {
                QLineEdit *line = qobject_cast<QLineEdit*>(valueInput);
                value = line->text();
            }
            model->updateVarInternal(nameInput->text(),value);
        }
    }
}

void EditorWindow::resizeWorkArea(int width, int height){
    workArea->setSizeWA(width, height);
}

void EditorWindow::workAreaLeftClick(QPoint position){
    statusBarLabel->setText("left: " + QString::number(position.x()) + ", " + QString::number(position.y()));
    //debug
    /*
    FSMTransition * arrow = new FSMTransition(workArea);
    arrow->setGeometry(workArea->rect()); // cover entire area
    arrow->show();
    */
    //end of debug
    if(isStateMoving){
        if(checkIfFSMFits(position, allStates[manipulatedState])){
            model->updateState(manipulatedState, position);
            allStates[manipulatedState] = nullptr;
        }else{
            statusBarLabel->setText("State did not fit");
        }
        isStateMoving = false;
    }

    if(isStateConnecting){
        isStateConnecting = false;
    }
}

void EditorWindow::workAreaRightClick(QPoint position){
    statusBarLabel->setText("right: " + QString::number(position.x()) + ", " + QString::number(position.y()));

    QMenu menu(this);  // create a QMenu

    QAction* addStateAction = menu.addAction("Add new state ...");
    connect(addStateAction, &QAction::triggered, this, [=]() {
        QString name = renamingWindow("Insert state");
        if(name != ""){
            if(allStates.contains(name)){
                QMessageBox::warning(this,"Cannot insert state","State cannot be insterted, because states need to have unique names.");
            }else{
                model->updateState(name, position);
            }
        }
    });
    if(!checkIfFSMFits(position)){
        addStateAction->setEnabled(false);
    }
    QAction* closeWindowAction = menu.addAction("Close program");
    connect(closeWindowAction, &QAction::triggered, this, &EditorWindow::close);

    QAction* startProjectionAction = menu.addAction("Start projection");

    QAction* renameFSMAction = menu.addAction("Rename FSM ...");
    connect(renameFSMAction, &QAction::triggered, this, [=](bool){
        QString name = renamingWindow("Renaming FSM");
        if(name != ""){
            model->renameFsm(name);
        }
    });

    QAction* resizeWorkareaAction = menu.addAction("Resize work-area ...");
    connect(resizeWorkareaAction, &QAction::triggered, this, [=](bool){resizeWorkArea();});

    QAction * loadFileAction = menu.addAction("Load file ...");
    connect(loadFileAction, &QAction::triggered, this, [this](bool){
        model->loadFile(QFileDialog::getOpenFileName());
    });

    QAction * saveFileAction = menu.addAction("Save file as ...");
    connect(saveFileAction, &QAction::triggered, this, [this](bool){
        model->saveFile(QFileDialog::getSaveFileName());
    });

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
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on

    QMenu menu(this);  // create a QMenu

    // Connect state to another state with transition
    QAction* connectToAction = menu.addAction("Connect to ...");
    connect(connectToAction, &QAction::triggered, this, [=](bool){
        isStateConnecting = true;
        manipulatedState = stateClicked->getName();
    });

    // Destroying state
    QAction* deleteAction = menu.addAction("Delete");
    connect(deleteAction, &QAction::triggered, this,[=](bool){
        model->destroyState(stateClicked->getName());
    });

    // Setting initial state
    QAction* setStartAction = menu.addAction("Set as starting");
    connect(setStartAction, &QAction::triggered, this, 
            [=](bool){
                model->updateActiveState(stateClicked->getName());
            }
        );

    // Edit state action
    QAction* editOutputAction = menu.addAction("Edit state action ...");
    connect(editOutputAction, &QAction::triggered, this, [=](bool){
        QDialog dialog(this);
        dialog.setWindowTitle("Editing output");

        QTextEdit *outputEdit = new QTextEdit(&dialog);
        outputEdit->setText(stateClicked->getOutput());

        // OK + Cancel buttons
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->addWidget(outputEdit);      // first the QTextEdit
        layout->addWidget(&buttonBox);

        if (dialog.exec() == QDialog::Accepted) {
            model->updateAction(stateClicked->getName(),outputEdit->toPlainText());
        }

    });

    // Rename a state
    QAction* renameStateAction = menu.addAction("Rename ...");
    connect(renameStateAction, &QAction::triggered, this, [=](bool){
        QString name = renamingWindow("Rename state");
        if(name != ""){
            model->updateStateName(stateClicked->getName(),name);
        }
    });

    // Move the state
    QAction * moveStateAction = menu.addAction("Move this state");
    connect(moveStateAction, &QAction::triggered, this, [=](bool){
        isStateMoving = true;
        manipulatedState = stateClicked->getName();
    });
    menu.exec(QCursor::pos());
}

void EditorWindow::stateFSMLeftClick(){
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on

    //debug
    //stateClicked->recolor("pink","teal");
    //stateClicked->setName("right-clicked");
    if(isStateConnecting){
        model->updateTransition(0,manipulatedState,stateClicked->getName());
        isStateConnecting = false;
    }
}

bool EditorWindow::checkIfFSMFits(QPoint position, StateFSMWidget * skip){
    bool canBeInserted = true;
    QPoint sizeWA = workArea->getSizeWA();
    QPoint sizeS; sizeS.setX(150); sizeS.setY(180); //TODO: fix if states can be multiple sizes
    //check if fits into workArea
    int sx = position.x() + sizeS.x();
    int sy = position.y() + sizeS.y();
    canBeInserted = canBeInserted && sx < sizeWA.x() && sy < sizeWA.y();
    //check for collision with other states
    for(StateFSMWidget * state: allStates){
        if(state == nullptr || state == skip){
            continue;
        }
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

void EditorWindow::variableToBeDeleted(enum variableType type){
    QDialog dialog(this);
    dialog.setWindowTitle("Deleting variable");

    QFormLayout form(&dialog);

    QComboBox * variableChoice = new QComboBox(&dialog);
    form.addRow("Delete:",variableChoice);
    QList<QString> keys = allVars[type].keys();
    variableChoice->addItem("...");
    for (const QString &key : keys) {
        variableChoice->addItem(key);
    }

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = variableChoice->currentText();
        if(name == "...")return;
        switch(type){
            case INPUTV:
                model->destroyVarInput(name);
                break;
            case OUTPUTV:
                model->destroyVarOutput(name);
                break;
            case INTERNALV:
                model->destroyVarInternal(name);
                break;
            default:
                break;
        }
    }
}


QString EditorWindow::renamingWindow(QString title){
        QDialog dialog(this);
        dialog.setWindowTitle(title);

        QFormLayout form(&dialog);

        QLineEdit *nameInput = new QLineEdit(&dialog);
        form.addRow("Name:", nameInput);

        // OK + Cancel buttons
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        form.addRow(&buttonBox);

        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "") {
            return nameInput->text();
        }else{
            return "";
        }
}

void EditorWindow::insertFSMState(QPoint position, QString name){
    StateFSMWidget * s = new StateFSMWidget(position,this);
    s->setParent(workArea);
    s->move(position);
    s->setName(name);
    s->show();
    connect(s, &StateFSMWidget::rightClick, this, &EditorWindow::stateFSMRightClick);
    connect(s, &StateFSMWidget::leftClick, this, &EditorWindow::stateFSMLeftClick);
    allStates.insert(name,s);
}

void EditorWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Closing program");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Do you want to save before closing program?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Close);

    /*
    QAbstractButton *closeButton = msgBox.button(QMessageBox::Close);
    if (closeButton) closeButton->setIcon(QIcon());
    */
    QAbstractButton *cancelButton = msgBox.button(QMessageBox::Cancel);
    if (cancelButton) cancelButton->setIcon(QIcon());

    msgBox.setDefaultButton(QMessageBox::Save);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Save) {
        QString file = QFileDialog::getSaveFileName();
        if(file != ""){
            model->saveFile(file);
            model->cleanup();
            event->accept();
        }else{
            event->ignore();
        }
    } else if (ret == QMessageBox::Close) {
        model->cleanup();
        event->accept();
    } else {
        event->ignore();
    }
}

