/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow.cpp
 * @author  xkadlet00
 * @author  xcervia00
 *
 * @brief window where FSM is edited
 *
 */

#include "view/editorwindow.h"
#include "ui_editorwindow.h"
#include "editorwindow.h"
#include "view/state_fsm_widget/statefsmwidget.h"
#include "view/logging_window/loggingwindow.h"
#include "view/input_event_edit/input_event_line_edit.h"
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
#include <QScreen>
#include <cstdint>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    // Network enabled?
    networkManager = new FsmNetworkManager(this);

    statusBarLabel = new QLabel("");
    statusBar()->addWidget(statusBarLabel);

    ui->menubar->setStyleSheet( "QMenu::item { padding: 0px 30px 0px 5px;}");

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
    stopInterpretationAct = ui->actionStopInterpret;
    startInterpretationAct = ui->actionStartInterpret;
    inputSubmitButton = ui->submitBtn;

    inputEventField = static_cast<InputEventLineEdit*>(ui->inputField);
    inputEventCombox = ui->inputSelect;
    outputEventField = ui->outputField;

    connect(startButton, &QPushButton::clicked, this, &EditorWindow::startButtonClick);
    connect(stopButton, &QPushButton::clicked, this, &EditorWindow::stopButtonClick);
    connect(inputSubmitButton, &QPushButton::clicked, this, &EditorWindow::submitInputClick);
    connect(inputEventField, &QLineEdit::returnPressed, this, &EditorWindow::submitInputClick);

    connect(inputEventField, &InputEventLineEdit::upArrowPressed, this, &EditorWindow::scrollInputComboxUp);
    connect(inputEventField, &InputEventLineEdit::downArrowPressed, this, &EditorWindow::scrollInputComboxDown);

    connect(inputEventCombox, &QComboBox::currentTextChanged, this, &EditorWindow::inputComboxChanged);

    // Don't enable buttons with space by default and handle it manually
    stopButton->setFocusPolicy(Qt::NoFocus);
    startButton->setFocusPolicy(Qt::NoFocus);
    this->stopInterpretationAct->setEnabled(false);
    this->startInterpretationAct->setEnabled(false);
    QAction* toggleInterpretation = new QAction("Toggle Interpret", this);
    this->addAction(toggleInterpretation);
    connect(toggleInterpretation, &QAction::triggered, this, &EditorWindow::handleActionToggleInterpretation);
    toggleInterpretation->setShortcut(QKeySequence(Qt::Key_Space));

    // === Logging Window ===
    loggingWindow = new LoggingWindow(this);
    loggingWindow->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    loggingWindow->setMinimumHeight(150);
    ui->verticalLayout->addWidget(loggingWindow);
    ui->verticalLayout->setStretch(0, 4);
    ui->verticalLayout->setStretch(1, 1);

    ui->workAreaLayout->setStretch(0, 6);
    ui->workAreaLayout->setStretch(1, 1);

    // === Workarea actions ===
    // Adding state
    QAction* addStateAction = new QAction("Add State", this);
    this->addAction(addStateAction);
    connect(addStateAction, &QAction::triggered, this, [this]() {this->handleActionAddState(workArea->mapFromGlobal(QCursor::pos()));});
    addStateAction->setShortcut(QKeySequence(Qt::Key_A));

    // Renaming FSM
    QAction* renameFsmAction = new QAction("Rename FSM", this);
    this->addAction(renameFsmAction);
    connect(renameFsmAction, &QAction::triggered, this, &EditorWindow::handleActionRenameFsm);
    renameFsmAction->setShortcut(QKeySequence(Qt::Key_F5));

    // Resize work area
    QAction* resizeWorkAreaAction = new QAction("Resize Work Area", this);
    this->addAction(resizeWorkAreaAction);
    connect(resizeWorkAreaAction, &QAction::triggered, this, &EditorWindow::handleActionResize);
    resizeWorkAreaAction->setShortcut(QKeySequence(Qt::Key_F9));

    // Rename state
    QAction* renameStateAction = new QAction("Rename State", this);
    this->addAction(renameStateAction);
    connect(renameStateAction, &QAction::triggered, this, [this]() {this->handleActionRenameState(this->getHoveredState());});
    renameStateAction->setShortcut(QKeySequence(Qt::Key_R));

    // Edit state
    QAction* editStateAction = new QAction("Edit State", this);
    this->addAction(editStateAction);
    connect(editStateAction, &QAction::triggered, this, [this]() {this->handleActionEditState(this->getHoveredState());});
    editStateAction->setShortcut(QKeySequence(Qt::Key_E));

    // Activate state
    QAction* activeStateAction = new QAction("Activate State", this);
    this->addAction(activeStateAction);
    connect(activeStateAction, &QAction::triggered, this, [this]() {this->handleActionActiveState(this->getHoveredState());});
    activeStateAction->setShortcut(QKeySequence(Qt::Key_S));

    // Delete state
    QAction* deleteStateAction = new QAction("Delete State", this);
    this->addAction(deleteStateAction);
    connect(deleteStateAction, &QAction::triggered, this, [this]() {this->handleActionDeleteState(this->getHoveredState());});
    deleteStateAction->setShortcut(QKeySequence(Qt::Key_D));

    // Connect state
    QAction* connectStateAction = new QAction("Connect State", this);
    this->addAction(connectStateAction);
    connect(connectStateAction, &QAction::triggered, this, [this]() {this->handleActionConnectState(this->getHoveredState());});
    connectStateAction->setShortcut(QKeySequence(Qt::Key_C));

    // === Menubar ===
    // = File =
    // Load
    connect(ui->actionLoad, &QAction::triggered, this, &EditorWindow::handleActionLoad);
    ui->actionLoad->setShortcut(QKeySequence::Open);
    // SaveAs
    connect(ui->actionSaveAs, &QAction::triggered, this, &EditorWindow::handleActionSaveAs);
    ui->actionSaveAs->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    // Save
    connect(ui->actionSave, &QAction::triggered, this, &EditorWindow::handleActionSave);
    ui->actionSave->setShortcut(QKeySequence::Save);
    // New
    connect(ui->actionNew, &QAction::triggered, this, &EditorWindow::handleActionNew);
    ui->actionNew->setShortcut(QKeySequence::New);
    // About
    connect(ui->actionAbout, &QAction::triggered, this, &EditorWindow::handleActionAbout);
    // Help
    connect(ui->actionHelp, &QAction::triggered, this, &EditorWindow::handleActionHelp);
    // Hotkeys
    connect(ui->actionHotkeys, &QAction::triggered, this, &EditorWindow::handleActionHotkeys);
    // Exit
    connect(ui->actionExit, &QAction::triggered, this, &EditorWindow::handleActionExit);

    // = Execute =
    connect(ui->actionStartInterpret, &QAction::triggered, this, &EditorWindow::startButtonClick);
    connect(ui->actionStopInterpret, &QAction::triggered, this, &EditorWindow::stopButtonClick);

    // = Right-click actions =
    // Moving 
    connect(workArea, &WorkArea::mouseMoved, this, &EditorWindow::workAreaMouseMoved);

    // === Network actions ===
    connect(ui->actionNetStartListening, &QAction::triggered, this, &EditorWindow::networkServerStart);
    connect(ui->actionNetStopListening, &QAction::triggered, this, &EditorWindow::networkServerStop);

    connect(ui->actionNetConnect, &QAction::triggered, this, &EditorWindow::networkClientStart);
    connect(ui->actionNetDisconnect, &QAction::triggered, this, &EditorWindow::networkClientStop);

    connect(ui->actionNetSettings, &QAction::triggered, this, &EditorWindow::networkSettings);
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
    cancelActionMove();
    cancelActionConnect();

    if(isInterpreting)
        return;


    // Register the network action
    if(this->networkManager != nullptr && this->isNetworking){
        if(networkManager->getInitiation() == false){
            NETWORK_ACTION(actionInterState(true));
        }
        
        if(networkManager->getState() == CLIENT && !networkManager->getInitiation())
        {
            return;
        }
    }

    isInterpreting = true;

    // Allow only variables set prior to interpretation
    QList<QString> keys = allVars[INPUTV].keys();
    for (const QString &key : keys) {
        this->inputEventCombox->addItem(key);
    }

    this->networkButtonsActivity(false);

    this->inputEventCombox->setEnabled(true);
    // this->inputSubmitButton->setEnabled(true); // Enable only once ... is not set

    this->stopButton->setEnabled(true);
    this->startButton->setEnabled(false);

    this->stopInterpretationAct->setEnabled(true);
    this->startInterpretationAct->setEnabled(false);

    // Disable variable editing
    this->variablesDisplay->setActButtonsAll(false);

    this->model->startInterpretation();
}

void EditorWindow::stopButtonClick()
{
    if(!isInterpreting)
        return;

    isInterpreting = false;
    // Remove all input events after interpretation ended
    this->model->stopInterpretation();

    // Register the network action
    NETWORK_ACTION(actionInterState(false));
    this->networkButtonsActivity(true);

    // Reset Input Combox/Submit button to disabled
    this->inputEventCombox->setEnabled(false);
    this->inputSubmitButton->setEnabled(false);
    this->stopButton->setEnabled(false);
    this->startButton->setEnabled(true);

    // Reset interpretation buttons in menu
    this->stopInterpretationAct->setEnabled(false);
    this->startInterpretationAct->setEnabled(true);

    // Reset input combox to default (...)
    this->inputEventCombox->clear();
    this->inputEventCombox->addItem("..."); // Add just ... by default

    // Clear outputs from current session
    this->outputEventField->clear();

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

    // Register the network action
    NETWORK_ACTION(actionInput(this->inputEventCombox->currentText(), this->inputEventField->text()));

    // Clear the event field
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
    // By default use current workdirectory, otherwise the last directory used
    auto startDirectory = lastFileName.isEmpty() ? QDir::currentPath() : QFileInfo(lastFileName).absolutePath();

    // Create Savefile dialog
    QFileDialog dialog(this, tr("Save FSM As..."));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectory(startDirectory);
    dialog.setNameFilter(tr("FSM Files (*.fsm);;All Files (*)"));
    dialog.setDefaultSuffix("fsm");    

    // Was the dialogue accepted?
    if(dialog.exec() == QDialog::Accepted){
        // Get the filename as the first of selected files
        QString fileName = dialog.selectedFiles().value(0);
        
        // Get the directory that was actually chosen
        lastFileName = fileName;

        // Save file and clear dirty flag
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
    
    // File was modified ==> Save changes?
    if(fileModified)
        this->promptOnModify();

    // Stop interpretation and remove active file
    this->stopButtonClick();
    fileModified = false;
    this->lastFileName.clear();

    // Blank workarea ==> clear all
    this->model->cleanup();
}

void EditorWindow::handleActionLoad()
{
    // If there are unsaved changes, prompt user to save them first
    if(fileModified){
        this->promptOnModify();
    }
    
    // Get name of file
    auto fileName = QFileDialog::getOpenFileName();
    if(fileName.isEmpty())
    {
        return;
    }

    // Stop interpretation and load file
    this->stopButtonClick();
    model->loadFile(fileName);
    
    // Update file state
    lastFileName = fileName;
    fileModified = false;
}

void EditorWindow::promptOnModify()
{
    // Setup prompt
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Opening new workplace");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Do you want to save before opening new file?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    // Add buttons
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

void EditorWindow::handleActionExit()
{
    this->close();
}

void EditorWindow::handleActionHelp()
{
    QMessageBox::information(this,"Help",
    QStringLiteral(
        "Right-click on work area to show allowed actions.\n\n"
        "Right panel is used for interpretaion purposes\n\nFile operations in the top left corner.\n\nEvents logged at bottom.")    
    );
}

void EditorWindow::handleActionAbout()
{
    QMessageBox::information(this,"About",
        QStringLiteral(
            "FSM interpreter and editor created as a project for ICP 2024/25 course at FIT VUT.\n\n"
            "Authors: Antonín Červinka, Tezera Kadlecová, Jana Zejdová\n")    
        );
}

void EditorWindow::handleActionHotkeys()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Hotkeys");

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);

    QScrollArea* scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollWidget = new QWidget();
    QFormLayout* form = new QFormLayout;
    scrollWidget->setLayout(form);


    // hotkeys
    QVector<QPair<QString, QString>> hotkeys = {
        {"<h2>STATE ACTIONS</h2>", ""},
        {"A", "Add new state"},
        {"C", "Connect states"},
        {"E", "Edit state action"},
        {"D", "Delete state"},
        {"S", "Set state to starting/initial"},
        {"R", "Rename state"},
        {"", ""},
        {"<h2>FSM ACTIONS</h2>",""},
        {"SPACE","Toggle interpretation"},
        {"CTRL + S","Save current FSM"},
        {"CTRL + SHIFT + S","Save current FSM to new file"},
        {"CTRL + O","Open FSM"},
        {"CTRL + N","New FSM"},
        {"F5","Rename FSM"},
        {"F9","Resize Work Area"}
    };

    for (const auto &hotkey : hotkeys) {
        form->addRow(new QLabel("<b>" + hotkey.first + "</b>"), new QLabel(hotkey.second));
    }

    // Finalize scroll area
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // OK button
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    mainLayout->addWidget(buttonBox);

    dialog.resize(500,300);
    dialog.exec();

}

void EditorWindow::handleActionRenameFsm()
{
    if(isInterpreting)
        return;

    QString name = renamingWindow("Renaming FSM");
    if(name != ""){
        model->renameFsm(name);
    }
}

void EditorWindow::handleActionResize()
{   
    if(isInterpreting)
        return;

    this->resizeWorkArea();
}

void EditorWindow::handleActionRenameState(StateFSMWidget* state)
{
    if(state == nullptr || isInterpreting) return;

    QString name = renamingWindow("Rename state");
    if(name != ""){
        model->updateStateName(state->getName(),name);
    }
}

void EditorWindow::handleActionEditState(StateFSMWidget *state)
{
    if(state == nullptr || isInterpreting) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Editing output");

    QTextEdit *outputEdit = new QTextEdit(&dialog);
    outputEdit->setText(state->getOutput());

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(outputEdit);      // first the QTextEdit
    layout->addWidget(&buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        model->updateAction(state->getName(),outputEdit->toPlainText());
    }
}

void EditorWindow::handleActionActiveState(StateFSMWidget *state)
{
    if(state == nullptr || isInterpreting) return;

    model->updateActiveState(state->getName());
}

void EditorWindow::handleActionDeleteState(StateFSMWidget *state)
{
    if(state == nullptr || isInterpreting) return;

    model->destroyState(state->getName());
}

void EditorWindow::handleActionAddState(QPoint position)
{
    if(isInterpreting)
        return;

    if(!checkIfFSMFits(position)){
        return;
    }

    QString name = renamingWindow("Insert state");
    if(name != ""){
        if(allStates.contains(name)){
            QMessageBox::warning(this,"Cannot insert state","State cannot be insterted, because states need to have unique names.");
        }else{
            model->updateState(name, position);
        }
    }
}

void EditorWindow::handleActionConnectState(StateFSMWidget *state)
{
    if(state == nullptr || isInterpreting) return;

    // Cursor to cross
    QApplication::setOverrideCursor(Qt::CrossCursor);    

    isStateConnecting = true;
    manipulatedState = state->getName();
}

void EditorWindow::handleActionMoveState(StateFSMWidget *movingState)
{
    if(movingState == nullptr || isInterpreting) return;

    isStateMoving = true;
    movingStateWidget = movingState;
    movingStateOrigPos = movingState->getPosition();
    manipulatedState = movingState->getName();

    for(const auto &val : allStates.values()){
        val->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }

    if (!ghostStateWidget) {
        ghostStateWidget = new QLabel(workArea);
        ghostStateWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    auto size = movingStateWidget->getSize();
    ghostStateWidget->setFixedSize(size.x(), size.y());
    ghostStateWidget->move(workArea->mapFromGlobal(workArea->mapToGlobal(movingStateOrigPos)));
    ghostStateWidget->show();
    ghostStateWidget->raise();

    movingStateWidget->hide(); // Maybe??

    QApplication::setOverrideCursor(Qt::SizeAllCursor);
    workArea->setMouseTracking(true);
}

/*
===========================
     VARIABLE RELATED 
===========================
*/

void EditorWindow::variableToBeEdited(enum variableType type){
    if(isStateConnecting){
        cancelActionConnect();
    }
    if(isStateMoving){
        cancelActionMove();
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
        cancelActionConnect();
    }

    if(isStateMoving){
        cancelActionMove();
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

        // rewriting value
        QPointer<QWidget> valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);

        // dynamic replacement (String variable has LineEdit, Bool variable has ComboBox, ...)
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
                    form.removeRow(2);
                    delete valueInput;
                }

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


    if(isStateMoving && movingStateWidget && ghostStateWidget){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        workArea->setMouseTracking(false);

        ghostStateWidget->hide();
        movingStateWidget->show();

        if(checkIfFSMFits(position, movingStateWidget)){
            // Reenable states
            for(const auto &val : allStates.values()){
                val->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }

            model->updateState(manipulatedState, position);
            manipulatedState.clear();
        }else{
            cancelActionMove();
            QMessageBox::warning(this,"Warning","State did not fit.");
        }
        isStateMoving = false;
    }

    if(isStateConnecting){
        cancelActionConnect();
    }
}

void EditorWindow::workAreaRightClick(QPoint position){
    if(isStateMoving){
        this->cancelActionMove();
        isStateMoving = false;
        return;
    }
    if(isStateConnecting){
        cancelActionConnect();
        return;
    }

    QMenu menu(this);  // create a QMenu

    QAction* addStateAction = menu.addAction("Add new state...");
    QAction* closeWindowAction = menu.addAction("Close program");
    QAction* renameFSMAction = menu.addAction("Rename FSM...");
    QAction* resizeWorkareaAction = menu.addAction("Resize work-area...");
    QAction* loadFileAction = menu.addAction("Load file...");
    QAction* saveFileAction = menu.addAction("Save file as...");

    if (isInterpreting){
        addStateAction->setEnabled(false);
        renameFSMAction->setEnabled(false);
        resizeWorkareaAction->setEnabled(false);
        loadFileAction->setEnabled(false);
        saveFileAction->setEnabled(false);
    }



    // adds new state
    connect(addStateAction, &QAction::triggered, this, [this, position]() {
        this->handleActionAddState(position);
    });

    if(!checkIfFSMFits(position)){
        addStateAction->setEnabled(false);
    }

    connect(closeWindowAction, &QAction::triggered, this, &EditorWindow::handleActionExit);

    // rename whole FSM
    connect(renameFSMAction, &QAction::triggered, this, &EditorWindow::handleActionRenameFsm);

    // resize work area
    connect(resizeWorkareaAction, &QAction::triggered, this, &EditorWindow::handleActionResize);


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

void EditorWindow::networkServerStart()
{
    if(this->networkManager == nullptr)
        return;

    isNetworking = true;

    qInfo() << "Network: Starting a server on " << this->networkManager->getServerInfo().address.toString() 
            << " via " << this->networkManager->getServerInfo().port;

    NETWORK_ACTION(enableServer());

    // Adjust buttons
    this->ui->actionNetSettings->setEnabled(false);
    this->ui->actionNetConnect->setEnabled(false);
    this->ui->actionNetDisconnect->setEnabled(false);
    this->ui->actionNetStartListening->setEnabled(false);
    this->ui->actionNetStopListening->setEnabled(true);
}

void EditorWindow::networkServerStop()
{
    if(this->networkManager == nullptr)
        return;

    qInfo() << "Network: Stopping a server on " << this->networkManager->getServerInfo().address.toString() 
            << " via " << this->networkManager->getServerInfo().port;

    NETWORK_ACTION(cancelServer());

    // Adjust buttons
    this->ui->actionNetSettings->setEnabled(true);
    this->ui->actionNetConnect->setEnabled(true);
    this->ui->actionNetDisconnect->setEnabled(false);
    this->ui->actionNetStartListening->setEnabled(true);
    this->ui->actionNetStopListening->setEnabled(false);

    isNetworking = false;
}

void EditorWindow::networkClientStart()
{
    if(this->networkManager == nullptr)
        return;

    isNetworking = true;

    qInfo() << "Network: Connecting to server " << this->networkManager->getServerInfo().address.toString() 
            << " via " << this->networkManager->getServerInfo().port;

    NETWORK_ACTION(enableClient());

    // Adjust buttons
    this->ui->actionNetSettings->setEnabled(false);
    this->ui->actionNetConnect->setEnabled(false);
    this->ui->actionNetDisconnect->setEnabled(true);
    this->ui->actionNetStartListening->setEnabled(false);
    this->ui->actionNetStopListening->setEnabled(false);
}

void EditorWindow::networkClientStop()
{
    if(this->networkManager == nullptr)
        return;

    qInfo() << "Network: Disconnecting from server " << this->networkManager->getServerInfo().address.toString() 
            << " via " << this->networkManager->getServerInfo().port;

    NETWORK_ACTION(cancelClient());

    // Adjust buttons
    this->ui->actionNetSettings->setEnabled(true);
    this->ui->actionNetConnect->setEnabled(true);
    this->ui->actionNetDisconnect->setEnabled(false);
    this->ui->actionNetStartListening->setEnabled(true);
    this->ui->actionNetStopListening->setEnabled(false);

    isNetworking = false;
}

void EditorWindow::networkSettings()
{
    if(this->networkManager == nullptr)
        return;

    auto &serverInfo = this->networkManager->getServerInfo();

    QDialog dialog(this);
    dialog.setWindowTitle("Network settings");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    
    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);

    QLabel *hostnameLabel = new QLabel(tr("Hostname: "), &dialog);
    QLineEdit *hostnameEdit = new QLineEdit(serverInfo.address.toString(), &dialog);
    formLayout->addRow(hostnameLabel, hostnameEdit);

    QLabel *portLabel = new QLabel(tr("Port: "), &dialog);
    QSpinBox *portEdit = new QSpinBox(&dialog);
    portEdit->setMinimum(1);
    portEdit->setMaximum(UINT16_MAX);
    portEdit->setValue(serverInfo.port);
    formLayout->addRow(portLabel, portEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);;

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        QHostAddress addr;
        
        if(hostnameEdit->text().isEmpty()){
            addr.setAddress(DEFAULT_ADDRESS);
        }
        else if(hostnameEdit->text() == "Any")
        {
            addr.setAddress(QHostAddress::AnyIPv4);
        }
        else{
            addr.setAddress(hostnameEdit->text());
        }

        quint16 port = (quint16)portEdit->value();

        this->networkManager->setAddress(addr, port);
        dialog.accept();
    });

    // Cancel
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Add buttonbox to widget
    mainLayout->addWidget(buttonBox);
    dialog.exec();
}

void EditorWindow::networkButtonsActivity(bool activate)
{
    if(this->networkManager == nullptr)
        return;

    qDebug() << this->networkManager->getState();
    switch(this->networkManager->getState())
    {
        case NETWORK_MANAGER_STATE::NONE:
            this->ui->actionNetStartListening->setEnabled(activate);
            this->ui->actionNetStopListening->setEnabled(false);
            this->ui->actionNetConnect->setEnabled(activate);
            this->ui->actionNetDisconnect->setEnabled(false);
            this->ui->actionNetSettings->setEnabled(activate);
            break;

        case NETWORK_MANAGER_STATE::SERVER:
            this->ui->actionNetStartListening->setEnabled(false);
            this->ui->actionNetStopListening->setEnabled(activate);
            this->ui->actionNetConnect->setEnabled(false);
            this->ui->actionNetDisconnect->setEnabled(false);
            this->ui->actionNetSettings->setEnabled(false);
            break;

        case NETWORK_MANAGER_STATE::CLIENT:
            this->ui->actionNetStartListening->setEnabled(false);
            this->ui->actionNetStopListening->setEnabled(false);
            this->ui->actionNetConnect->setEnabled(false);
            this->ui->actionNetDisconnect->setEnabled(activate);
            this->ui->actionNetSettings->setEnabled(false);
            break;

        default:
            break;
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
        cancelActionConnect();
    }

    if(isStateMoving){
        cancelActionMove();
    }

    if(isInterpreting){
        return;
    }

    QMenu menu(this);  // create a QMenu

    QAction* renameStateAction = menu.addAction("Rename...");
    QAction* editOutputAction = menu.addAction("Edit state action...");
    QAction* connectToAction = menu.addAction("Connect to...");
    QAction* setStartAction = menu.addAction("Set as starting");
    QAction* moveStateAction = menu.addAction("Move state");
    QAction* deleteAction = menu.addAction("Delete");

    // Connect state to another state with transition
    connect(connectToAction, &QAction::triggered, this, [this, stateClicked](bool){
        this->handleActionConnectState(stateClicked);        
    });

    // Destroying state
    connect(deleteAction, &QAction::triggered, this,[this, stateClicked](bool){
        this->handleActionDeleteState(stateClicked);
    });

    // Setting initial state
    connect(setStartAction, &QAction::triggered, this, [this, stateClicked](bool){
        this->handleActionActiveState(stateClicked);
    });

    // Edit state action
    connect(editOutputAction, &QAction::triggered, this, [this, stateClicked](bool){
        this->handleActionEditState(stateClicked);
    });

    // Rename a state
    connect(renameStateAction, &QAction::triggered, this, [this, stateClicked](bool){
        this->handleActionRenameState(stateClicked);
    });

    // Move the state
    connect(moveStateAction, &QAction::triggered, this, [this, stateClicked](bool){
        this->handleActionMoveState(stateClicked);
    });
    menu.exec(QCursor::pos());
}

void EditorWindow::stateFSMLeftClick(){
    StateFSMWidget* stateClicked = qobject_cast<StateFSMWidget*>(sender()); // get state user clicked on
    if(stateClicked == nullptr) return;

    // Finalized connection
    if(isStateConnecting){
        model->updateTransition(0, manipulatedState, stateClicked->getName());
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        isStateConnecting = false;
        return;
    }

    if(!isStateMoving && !isInterpreting)
    {
        this->handleActionMoveState(stateClicked);
    }
}

bool EditorWindow::checkIfFSMFits(QPoint position, StateFSMWidget * skip){
    bool canBeInserted = true;
    QPoint sizeWA = workArea->getSizeWA();
    QPoint sizeS; sizeS.setX(160); sizeS.setY(200);
    //check if fits into workArea
    int sx = position.x() + sizeS.x();
    int sy = position.y() + sizeS.y();
    canBeInserted = canBeInserted && sx < sizeWA.x() && sy < sizeWA.y() && sx > sizeS.x() && sy > sizeS.y();
    //check for collision with other states

    for(StateFSMWidget * state: allStates){
        if(canBeInserted == false) {
            break;
        }

        if(state == nullptr || state == skip){
            continue;
        }
        QPoint fPos = state->getPosition();
        QPoint fSize = state->getSize();
        int fsx = fPos.x() + fSize.x();
        int fsy = fPos.y() + fSize.y();
        canBeInserted = canBeInserted && !(
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
        cancelActionMove();
    }

    if(isStateConnecting){
        cancelActionConnect();
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

        // Get Mouse pos

        QPoint mousePos = QCursor::pos();
        QSize dialogSize = dialog.size();

        int dialogX = mousePos.x() - ((dialogSize.width()));
        int dialogY = mousePos.y() - ((dialogSize.height()));
        QPoint dialogPos(dialogX, dialogY);

        QRect screenGeometry = qApp->primaryScreen()->geometry();
        if (dialogPos.x() < screenGeometry.left()) {
            dialogPos.setX(screenGeometry.left());
        } else if (dialogPos.x() + dialogSize.width() > screenGeometry.right()) {
            dialogPos.setX(screenGeometry.right() - dialogSize.width());
        }
        if (dialogPos.y() < screenGeometry.top()) {
            dialogPos.setY(screenGeometry.top());
        } else if (dialogPos.y() + dialogSize.height() > screenGeometry.bottom()) {
            dialogPos.setY(screenGeometry.bottom() - dialogSize.height());
        }
        dialog.move(dialogPos);

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
    //this->startButton->setEnabled(true);

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
        model->stopInterpretation();
        model->cleanup();
        event->accept();
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Closing program");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Do you want to save before closing program?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Close);

    QAbstractButton *cancelButton = msgBox.button(QMessageBox::Cancel);
    if (cancelButton) cancelButton->setIcon(QIcon());

    msgBox.setDefaultButton(QMessageBox::Save);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Save) {
        this->handleActionSave();
    } else if (ret == QMessageBox::Close) {
        model->stopInterpretation();
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

    QDialogButtonBox *buttonBox;
    if(isInterpreting){
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        //conditionEdit->setStyleSheet("background-color: transparent");
        //conditionEdit->setFrame(false);
    }else{
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        QPushButton *deleteButton = new QPushButton("Delete", &dialog);
        buttonBox->addButton(deleteButton, QDialogButtonBox::DestructiveRole);

        // OK
        connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
            size_t selectedId = idComboBox->currentData().toULongLong();
            model->updateCondition(selectedId, conditionEdit->text());
            dialog.accept();
        });

        // Delete
        connect(deleteButton, &QPushButton::clicked, [&]() {
            size_t selectedId = idComboBox->currentData().toULongLong();
            transition->subTransition(selectedId);
            model->destroyTransition(selectedId);
            dialog.accept();
        });
    }

    // Cancel
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    mainLayout->addWidget(buttonBox);

    // Block edits when interpretation is ON
    if(isInterpreting)
    {  
        conditionEdit->setReadOnly(true);   
    }

    // change fields when a different ID is selected
    auto updateFields = [=]() {
        size_t selectedId = idComboBox->currentData().toULongLong(); // convert QVariant to size_t
        const auto &cond = allTransitionsConditions[selectedId];
        infoLabel->setText(cond.src + " -> " + cond.dest);
        conditionEdit->setText(cond.condition);
    };
    updateFields();
    connect(idComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), updateFields);

    dialog.exec();
}

/*
===========================
      HOTKEY RELATED 
===========================
*/

void EditorWindow::scrollInputComboxUp()
{
    if(this->inputEventCombox->count() <= 1) return;
    this->inputEventCombox->setCurrentIndex((this->inputEventCombox->currentIndex() + 1) % this->inputEventCombox->count());
}

void EditorWindow::scrollInputComboxDown()
{
    if(this->inputEventCombox->count() <= 1) return;

    if(this->inputEventCombox->currentIndex() == 0)
    {
        this->inputEventCombox->setCurrentIndex(this->inputEventCombox->count() - 1);
    }
    else
    {
        this->inputEventCombox->setCurrentIndex(this->inputEventCombox->currentIndex() - 1);
    }
    
}

void EditorWindow::movementUpdateTransitions()
{
    for(const auto &key : allTransitionsUI.keys()){
        StateFSMWidget* src = allStates.value(key.first, nullptr);
        StateFSMWidget* dst = allStates.value(key.second, nullptr);

        if(src == movingStateWidget || dst == movingStateWidget){
            if(src && dst){
                QPoint srcPos, dstPos;
                QPoint srcSize, dstSize;

                if(src == dst)
                {
                    srcPos = ghostStateWidget->pos();
                    srcSize = QPoint(ghostStateWidget->size().width(), ghostStateWidget->size().height());
                    dstPos = srcPos;
                    dstSize = srcSize;
                }
                else if(movingStateWidget == src)
                {
                    srcPos = ghostStateWidget->pos();
                    srcSize = QPoint(ghostStateWidget->size().width(), ghostStateWidget->size().height());
                    dstPos = dst->getPosition();
                    dstSize = dst->getSize();
                }
                else
                {
                    srcPos = src->getPosition();
                    srcSize = src->getSize();
                    dstPos = ghostStateWidget->pos();
                    dstSize = QPoint(ghostStateWidget->size().width(), ghostStateWidget->size().height());
                }

                if(src && dst){
                    allTransitionsUI[key]->relocateTransition(srcPos, srcSize, dstPos, dstSize);
                }
            }
        }
    }
}

void EditorWindow::handleActionToggleInterpretation()
{
    if(isInterpreting)
    {
        if(this->stopButton->isEnabled()){
            this->stopButtonClick();
        }
    }
    else
    {
        if(this->startButton->isEnabled()){
            this->startButtonClick();
        }
    }
}

void EditorWindow::workAreaMouseMoved(QPoint pos) {
    if (isStateMoving && ghostStateWidget && movingStateWidget) {
  
        ghostStateWidget->move(pos);

        this->movementUpdateTransitions();

        if (checkIfFSMFits(pos, movingStateWidget)) {
            ghostStateWidget->setStyleSheet("QLabel { border: 2px dashed green; background-color: rgba(200, 255, 200, 100); }");
        } else {
            ghostStateWidget->setStyleSheet("QLabel { border: 2px dashed red; background-color: rgba(255, 200, 200, 100); }");
        }
    }
}

void EditorWindow::cancelActionMove()
{
    if (isStateMoving && movingStateWidget) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        workArea->setMouseTracking(false);

        for(const auto &val : allStates.values()){
            val->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        }

        if (ghostStateWidget) {
            ghostStateWidget->move(movingStateOrigPos);
            ghostStateWidget->hide();
        }
        movingStateWidget->move(movingStateOrigPos);
        movingStateWidget->show();
        movementUpdateTransitions();

        isStateMoving = false;
        movingStateWidget = nullptr;
    }
}

void EditorWindow::cancelActionConnect()
{
    if(isStateConnecting)
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        isStateConnecting = false;
    }
}

StateFSMWidget* EditorWindow::getHoveredState()
{
    // Get widget below mouse
    QWidget* currentWidget = workArea->childAt(workArea->mapFromGlobal(QCursor::pos()));

    StateFSMWidget* stateHovered = nullptr;
    while (currentWidget != nullptr && currentWidget != this->workArea) {
        stateHovered = qobject_cast<StateFSMWidget*>(currentWidget);
        if (stateHovered) {
            break;
        }
        currentWidget = currentWidget->parentWidget();
    }
    return stateHovered;
}
