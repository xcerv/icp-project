#include "variablesdisplay.h"
#include <qdialog.h>
#include <qlineedit.h>
#include <qdialogbuttonbox.h>
#include "ui_variablesdisplay.h"

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

FSMVariable VariablesDisplay::insertVariable(enum variableType type, QString name, QString value){
    QFormLayout *layout = qobject_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());

    QLabel *newLblName = new QLabel(name, ui->scrollAreaWidgetContents);
    QLabel *newLblValue = new QLabel(value, ui->scrollAreaWidgetContents);

    int row;
    layout->getWidgetPosition(typeVar[type], &row, nullptr);
    layout->insertRow(row + 1, newLblName, newLblValue);
    FSMVariable h{newLblName,newLblValue};
    return h;
}


void VariablesDisplay::getVariableInfoInsert(enum variableType type){
    emit addVariableToDisplay(type);
}

void VariablesDisplay::getVariableInfoDelete(enum variableType type){
    emit removeVariableFromDisplay(type);
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
