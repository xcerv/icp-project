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
#include "qdebug.h"
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
#include "editorwindow.h"

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    statusBarLabel = new QLabel("");
    statusBar()->addWidget(statusBarLabel);

    // Variable display
    variablesDisplay = new VariablesDisplay(this);
    variablesDisplay->move(15, 35); // top-left corner
    variablesDisplay->raise(); // appearing above other widgets

    if(variablesDisplay->size().width() > this->size().width()/2)
        variablesDisplay->setDisplayVisibility(false);

    connect(variablesDisplay, &VariablesDisplay::addVariableToDisplay, this, &EditorWindow::variableToBeAdded);
    connect(variablesDisplay, &VariablesDisplay::removeVariableFromDisplay, this, &EditorWindow::variableToBeDeleted);
    connect(variablesDisplay, &VariablesDisplay::editVariableInDisplay, this, &EditorWindow::variableToBeEdited);

    // WorkAreaContrainer
    workAreaScrollContainer = new QWidget();
    workAreaScrollLayout = new QVBoxLayout(workAreaScrollContainer);  // layout for centering
    workAreaScrollContainer->setLayout(workAreaScrollLayout);
    ui->workAreaScroll->setWidget(workAreaScrollContainer);  // add container to scroll area
    ui->workAreaScroll->setWidgetResizable(true);

    // === Workarea ===
    workArea = new WorkArea;
    connect(workArea, &WorkArea::leftClick, this, &EditorWindow::workAreaLeftClick);
    connect(workArea, &WorkArea::rightClick, this, &EditorWindow::workAreaRightClick);
    resizeWorkArea(1500,700);
    workAreaScrollLayout->addWidget(workArea);
    workAreaScrollLayout->setAlignment(workArea, Qt::AlignHCenter);

    // === Interpreter window ===
    // Link important elements to attributes
    stopButton = ui->stopBtn;
    startButton = ui->startBtn;
    inputSubmitButton = ui->submitBtn;

    inputEventField = ui->inputField;
    inputEventCombox = ui->inputSelect;
    outputEventField = ui->outputField;

    connect(startButton, &QPushButton::clicked, this, &EditorWindow::startButtonClick);
    connect(stopButton, &QPushButton::clicked, this, &EditorWindow::stopButtonClick);
    connect(inputSubmitButton, &QPushButton::clicked, this, &EditorWindow::submitInputClick);

    connect(inputEventCombox, &QComboBox::currentTextChanged, this, &EditorWindow::inputComboxChanged);

    // === Logging Window ===
    loggingWindow = new LoggingWindow(this);
    loggingWindow->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    loggingWindow->setMinimumHeight(150);
    ui->verticalLayout->addWidget(loggingWindow);
    ui->verticalLayout->setStretch(0, 4);
    ui->verticalLayout->setStretch(1, 1);

    ui->workAreaLayout->setStretch(0, 6);
    ui->workAreaLayout->setStretch(1, 1);

    // === Menubar ===
    // Load
    connect(ui->actionLoad, &QAction::triggered, this, &EditorWindow::handleActionLoad);
    // SaveAs
    connect(ui->actionSaveAs, &QAction::triggered, this, &EditorWindow::handleActionSaveAs);
    // Save
    connect(ui->actionSave, &QAction::triggered, this, &EditorWindow::handleActionSave);
    // New
    connect(ui->actionNew, &QAction::triggered, this, &EditorWindow::handleActionNew);
    // About
    connect(ui->actionAbout, &QAction::triggered, this, &EditorWindow::handleActionAbout);
    // Help
    connect(ui->actionHelp, &QAction::triggered, this, &EditorWindow::handleActionHelp);

}

EditorWindow::~EditorWindow()
{
    delete ui;
}


/*
===========================
     INTERPRETER RELATED 
===========================
*/

void EditorWindow::startButtonClick()
{
    // Allow only variables set prior to interpretation
    QList<QString> keys = allVars[INPUTV].keys();
    for (const QString &key : keys) {
        this->inputEventCombox->addItem(key);
    }

    this->inputEventCombox->setEnabled(true);
    // this->inputSubmitButton->setEnabled(true); // Enable only once ... is not set

    this->stopButton->setEnabled(true);
    this->startButton->setEnabled(false);

    // Disable variable editing
    this->variablesDisplay->setActButtonsAll(false);

    this->model->startInterpretation();
}

void EditorWindow::stopButtonClick()
{
    // Remove all input events after interpretation ended
    this->model->stopInterpretation();

    // Reset Input Combox/Submit button to disabled
    this->inputEventCombox->setEnabled(false);
    this->inputSubmitButton->setEnabled(false);
    this->stopButton->setEnabled(false);
    this->startButton->setEnabled(true);

    this->inputEventCombox->clear();
    this->inputEventCombox->addItem("..."); // Add just ... by default

    // Enable variable editing
    for(int i = 0; i < NUMV; i++){
        this->variablesDisplay->setActButtonsAdding(!allVars[i].isEmpty(), (variableType)i);
    }
}

void EditorWindow::submitInputClick()
{
    // Don't allow empty inputs
    if(this->inputEventField->text().isEmpty() || this->inputEventCombox->currentText() == "...")
        return;

    this->model->inputEvent(this->inputEventCombox->currentText(), this->inputEventField->text());
    this->inputEventField->clear();
}

void EditorWindow::inputComboxChanged()
{
    if(this->inputEventCombox->currentText() == "...")
    {
        this->inputSubmitButton->setEnabled(false);
    }
    else
    {
        this->inputSubmitButton->setEnabled(true);
    }
}

void EditorWindow::handleActionSaveAs()
{
    auto startDirectory = lastFileName.isEmpty() ? QDir::currentPath() : QFileInfo(lastFileName).absolutePath();

    QFileDialog dialog(this, tr("Save FSM As..."));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectory(startDirectory);
    dialog.setNameFilter(tr("FSM Files (*.fsm);;All Files (*)"));
    dialog.setDefaultSuffix("fsm");    

    if(dialog.exec() == QDialog::Accepted){
        QString fileName = dialog.selectedFiles().value(0);
        
        // Get the directory that was actually chosen
        lastFileName = fileName;

        fileModified = false;
        model->saveFile(fileName);
    }
}

void EditorWindow::handleActionSave()
{
    // Saved for first time, handle as "Save as"
    if(lastFileName.isEmpty())
    {
        this->handleActionSaveAs();
        return;
    }

    // Nothing was changed since last time, do nothing
    if(!fileModified)
        return;

    // Save to last file
    fileModified = false;
    this->saveFile(this->lastFileName);
}

void EditorWindow::handleActionNew()
{
    // Creating blank file multiple times
    if(!this->fileModified && lastFileName.isEmpty())
        return;

    fileModified = false;
    this->lastFileName.clear();
    this->model->cleanup();
}

void EditorWindow::handleActionLoad()
{
    // If there are unsaved changes, prompt user to save them first
    if(fileModified){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Opening new workplace");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Do you want to save before opening new file?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        QAbstractButton *cancelButton = msgBox.button(QMessageBox::Cancel);
        if (cancelButton) 
            cancelButton->setIcon(QIcon());

        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        // User changed their mind - no opening
        if(ret == QMessageBox::Cancel){
            return;
        }

        // User decided to save first
        if (ret == QMessageBox::Save) {
            this->handleActionSave();
        }
    }
    
    auto fileName = QFileDialog::getOpenFileName();
    if(fileName.isEmpty())
    {
        return;
    }

    model->loadFile(fileName);
    
    lastFileName = fileName;
    fileModified = false;
}

void EditorWindow::handleActionExit()
{
    this->close();
}

void EditorWindow::handleActionHelp()
{
    QMessageBox::information(this,"Help",
    QStringLiteral("Right-click on work area to show allowed actions.\n\n"
        "Right panel is used for interpretaion purposes\n\nFile operations in the top left corner.\n\nEvents logged at bottom.")    
    );
}

void EditorWindow::handleActionAbout()
{
    QMessageBox::information(this,"About",
        QStringLiteral("FSM interpreter and editor created as a project for ICP 2024/25 course at FIT VUT.\n\nAuthors: Antonín Červinka, Tezera Kadlecová, Jana Zejdová\n")    
        );
}

/*
===========================
     VARIABLE RELATED 
===========================
*/

void EditorWindow::variableToBeEdited(enum variableType type){
    if(isStateConnecting){
        isStateConnecting = false;
    }
    if(isStateMoving){
        isStateMoving = false;
    }

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
    if(isStateConnecting){
        isStateConnecting = false;
    }

    if(isStateMoving){
        isStateMoving = false;
    }

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
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "") {
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
    statusBarLabel->setText(QString::number(position.x()) + ", " + QString::number(position.y()));
    if(isStateMoving){
        if(checkIfFSMFits(position, allStates[manipulatedState])){
            model->updateState(manipulatedState, position);
            manipulatedState = nullptr;
        }else{
            QMessageBox::warning(this,"Warning","State did not fit.");
        }
        isStateMoving = false;
    }

    if(isStateConnecting){
        isStateConnecting = false;
    }
}

void EditorWindow::workAreaRightClick(QPoint position){
    if(isStateMoving){
        isStateMoving = false;
    }
    if(isStateConnecting){
        isStateConnecting = false;
    }

    QMenu menu(this);  // create a QMenu

    QAction* addStateAction = menu.addAction("Add new state ...");
    QAction* closeWindowAction = menu.addAction("Close program");
    QAction* renameFSMAction = menu.addAction("Rename FSM ...");
    QAction* resizeWorkareaAction = menu.addAction("Resize work-area ...");
    QAction* loadFileAction = menu.addAction("Load file ...");
    QAction* saveFileAction = menu.addAction("Save file as ...");

    if (isInterpreting){
        addStateAction->setEnabled(false);
        renameFSMAction->setEnabled(false);
        resizeWorkareaAction->setEnabled(false);
        loadFileAction->setEnabled(false);
        saveFileAction->setEnabled(false);
    }



    // adds new state
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

    connect(closeWindowAction, &QAction::triggered, this, &EditorWindow::handleActionExit);

    // rename whole FSM
    connect(renameFSMAction, &QAction::triggered, this, [=](bool){
        QString name = renamingWindow("Renaming FSM");
        if(name != ""){
            model->renameFsm(name);
        }
    });

    // resize work area
    connect(resizeWorkareaAction, &QAction::triggered, this, [=](bool){resizeWorkArea();});


    connect(loadFileAction, &QAction::triggered, this, &EditorWindow::handleActionLoad);


    connect(saveFileAction, &QAction::triggered, this, &EditorWindow::handleActionSaveAs);


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

    if(isStateConnecting){
        isStateConnecting = false;
    }

    if(isStateMoving){
        isStateMoving = false;
    }

    if(isInterpreting){
        return;
    }

    QMenu menu(this);  // create a QMenu

    QAction* renameStateAction = menu.addAction("Rename ...");
    QAction* editOutputAction = menu.addAction("Edit state action ...");
    QAction* connectToAction = menu.addAction("Connect to ...");
    QAction* setStartAction = menu.addAction("Set as starting");
    QAction* moveStateAction = menu.addAction("Move state");
    QAction* deleteAction = menu.addAction("Delete");

    // Connect state to another state with transition
    connect(connectToAction, &QAction::triggered, this, [=](bool){
        isStateConnecting = true;
        manipulatedState = stateClicked->getName();
    });

    // Destroying state
    connect(deleteAction, &QAction::triggered, this,[=](bool){
        model->destroyState(stateClicked->getName());
    });

    // Setting initial state
    connect(setStartAction, &QAction::triggered, this, 
            [=](bool){
                model->updateActiveState(stateClicked->getName());
            }
        );

    // Edit state action
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
    connect(renameStateAction, &QAction::triggered, this, [=](bool){
        QString name = renamingWindow("Rename state");
        if(name != ""){
            model->updateStateName(stateClicked->getName(),name);
        }
    });

    // Move the state
    connect(moveStateAction, &QAction::triggered, this, [=](bool){
        isStateMoving = true;
        manipulatedState = stateClicked->getName();
    });
    menu.exec(QCursor::pos());
}

void EditorWindow::stateFSMLeftClick(){
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on
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
    if(isStateMoving){
        isStateMoving = false;
    }

    if(isStateConnecting){
        isStateConnecting = false;
    }
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
    // Enable interpretation only once at least one state exits
    this->startButton->setEnabled(true);

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
    // Nothing was changed, so dont show the message
    if(fileModified == false){
        event->accept();
        return;
    }

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
        this->handleActionSave();
    } else if (ret == QMessageBox::Close) {
        model->cleanup();
        event->accept();
    } else {
        event->ignore();
    }
}



void EditorWindow::editTransitionHanling(FSMTransition * transition){
    QDialog dialog(this);
    dialog.setWindowTitle("Edit conditions");
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QComboBox *idComboBox = new QComboBox(&dialog);
    auto c = transition->getTransitions();
    QList<size_t> idList = c.values();
    for (size_t tId : idList) {
        idComboBox->addItem(QString::number(tId), QVariant::fromValue(tId));
    }
    mainLayout->addWidget(idComboBox);

    QLabel *infoLabel = new QLabel(&dialog);
    mainLayout->addWidget(infoLabel);

    QLineEdit *conditionEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(conditionEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    QPushButton *deleteButton = new QPushButton("Delete", &dialog);
    buttonBox->addButton(deleteButton, QDialogButtonBox::DestructiveRole);
    mainLayout->addWidget(buttonBox);

    // change fields when a different ID is selected
    auto updateFields = [=]() {
        size_t selectedId = idComboBox->currentData().toULongLong(); // convert QVariant to size_t
        const auto &cond = allTransitionsConditions[selectedId];
        infoLabel->setText(cond.src + " -> " + cond.dest);
        conditionEdit->setText(cond.condition);
    };
    updateFields();
    connect(idComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), updateFields);

    // OK
    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        size_t selectedId = idComboBox->currentData().toULongLong();
        model->updateCondition(selectedId, conditionEdit->text());
        dialog.accept();
    });

    // Cancel
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Delete
    connect(deleteButton, &QPushButton::clicked, [&]() {
        size_t selectedId = idComboBox->currentData().toULongLong();
        transition->subTransition(selectedId);
        model->destroyTransition(selectedId);
        dialog.accept();
    });

    dialog.exec();
}

