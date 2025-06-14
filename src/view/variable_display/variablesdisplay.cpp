/**
 * Project name: ICP Project 2024/2025
 *
 * @file variablesdisplay.cpp
 * @author  xkadlet00
 *
 * @brief Defines the disply of input/output/internal variables
 *
 */

#include "view/variable_display/variablesdisplay.h"
#include <qdialog.h>
#include <qlineedit.h>
#include <qdialogbuttonbox.h>
#include "ui_variablesdisplay.h"
#include "variablesdisplay.h"

VariablesDisplay::VariablesDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VariablesDisplay)
{
    ui->setupUi(this);

    typeVar[INPUTV] = ui->lblInputVar;
    typeVar[OUTPUTV] = ui->lblOutputVar;
    typeVar[INTERNALV] = ui->lblInternalVar;

    connect(ui->btnHide, &QPushButton::clicked,this, &VariablesDisplay::hideOrShow);

    connect(ui->btnAddOutputVar, &QPushButton::clicked, this, [this](){getVariableInfoInsert(OUTPUTV);});
    connect(ui->btnAddInputVar, &QPushButton::clicked, this, [this](){getVariableInfoInsert(INPUTV);});
    connect(ui->btnAddInternalVar, &QPushButton::clicked, this, [this](){getVariableInfoInsert(INTERNALV);});

    connect(ui->btnRemoveInternalVar, &QPushButton::clicked, this, [this](){getVariableInfoDelete(INTERNALV);});
    connect(ui->btnRemoveInputVar, &QPushButton::clicked, this, [this](){getVariableInfoDelete(INPUTV);});
    connect(ui->btnRemoveOutputVar, &QPushButton::clicked, this, [this](){getVariableInfoDelete(OUTPUTV);});

    connect(ui->btnEditInputVar, &QPushButton::clicked, this, [this](){getVariableInfoEdit(INPUTV);});
    connect(ui->btnEditInternalVar, &QPushButton::clicked, this, [this](){getVariableInfoEdit(INTERNALV);});


    ui->btnRemoveInputVar->setDisabled(true);
    ui->btnRemoveOutputVar->setDisabled(true);
    ui->btnRemoveInternalVar->setDisabled(true);

    ui->btnEditInputVar->setDisabled(true);
    ui->btnEditInternalVar->setDisabled(true);
}

VariablesDisplay::~VariablesDisplay()
{
    delete ui;
    /*
    for(int i = 0; i < NUMV; i++){
        for(FSMVariable v : allVars[i]){
            delete v.name;
            delete v.value;
        }
    }
    */
}

void VariablesDisplay::hideOrShow(){
    shown = !shown;
    if(!shown){
        ui->btnHide->setText("show");
        setFixedSize(42,22);
    }else{
        ui->btnHide->setText("hide");
        setFixedSize(400,300);
    }
    ui->scrollArea->setVisible(shown);
}

void VariablesDisplay::setDisplayVisibility(bool visibility){
    shown = visibility;
    if(!shown){
        ui->btnHide->setText("show");
        setFixedSize(42,22);
    }else{
        ui->btnHide->setText("hide");
        setFixedSize(400,300);
    }
    ui->scrollArea->setVisible(shown);
}

FSMVariable VariablesDisplay::insertVariable(enum variableType type, QString name, QString value){
    QFormLayout *layout = qobject_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());

    QLabel *newLblName = new QLabel(name, ui->scrollAreaWidgetContents);
    QLabel *newLblValue = new QLabel(value, ui->scrollAreaWidgetContents);

    int row;
    layout->getWidgetPosition(typeVar[type], &row, nullptr);
    layout->insertRow(row + 1, newLblName, newLblValue);
    FSMVariable h{.name = newLblName, .value = newLblValue};
    return h;
}


void VariablesDisplay::getVariableInfoInsert(enum variableType type){
    emit addVariableToDisplay(type);
}

void VariablesDisplay::getVariableInfoDelete(enum variableType type){
    emit removeVariableFromDisplay(type);
}

void VariablesDisplay::getVariableInfoEdit(enum variableType type){
    emit editVariableInDisplay(type);
}

void VariablesDisplay::setActButtons(bool activate, enum variableType type){
    activate = !activate;
    if(type == INPUTV){
        ui->btnRemoveInputVar->setDisabled(activate);
        ui->btnEditInputVar->setDisabled(activate);
    }else if(type == OUTPUTV){
        ui->btnRemoveOutputVar->setDisabled(activate);
    }else if(type == INTERNALV){
        ui->btnRemoveInternalVar->setDisabled(activate);
        ui->btnEditInternalVar->setDisabled(activate);
    }
}

void VariablesDisplay::setActButtonsAdding(bool activate, variableType type){
    ui->btnAddInputVar->setEnabled(true);
    ui->btnAddOutputVar->setEnabled(true);
    ui->btnAddInternalVar->setEnabled(true);

    switch(type)
    {
        case INPUTV:
            ui->btnRemoveInputVar->setEnabled(activate);
            ui->btnEditInputVar->setEnabled(activate);
            break;

        case OUTPUTV:
            ui->btnRemoveOutputVar->setEnabled(activate);
            break;

        case INTERNALV:
            ui->btnRemoveInternalVar->setEnabled(activate);
            ui->btnEditInternalVar->setEnabled(activate);
            break;

        default:
            break;
    }
}

void VariablesDisplay::setActButtonsAll(bool activate){
    // Input
    ui->btnAddInputVar->setEnabled(activate);
    ui->btnEditInputVar->setEnabled(activate);
    ui->btnRemoveInputVar->setEnabled(activate);


    // Output
    ui->btnAddOutputVar->setEnabled(activate);
    ui->btnRemoveOutputVar->setEnabled(activate);

    // Internal
    ui->btnAddInternalVar->setEnabled(activate);
    ui->btnRemoveInternalVar->setEnabled(activate);
    ui->btnEditInternalVar->setEnabled(activate);
}