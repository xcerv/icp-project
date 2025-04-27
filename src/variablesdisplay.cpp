#include "variablesdisplay.h"
#include "ui_variablesdisplay.h"

VariablesDisplay::VariablesDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VariablesDisplay)
{
    ui->setupUi(this);
    connect(ui->btnHide, &QPushButton::clicked,this, &VariablesDisplay::hideOrShow);
    connect(ui->btnAddOutputVar, &QPushButton::clicked, this, [this](){insertVariable(ui->lblOutputVar);});
    connect(ui->btnAddInputVar, &QPushButton::clicked, this, [this](){insertVariable(ui->lblInputVar);});
    connect(ui->btnAddInternalVar, &QPushButton::clicked, this, [this](){insertVariable(ui->lblInternalVar);});
    ui->btnRemoveInputVar->setDisabled(true);
    ui->btnRemoveOutputVar->setDisabled(true);
    ui->btnRemoveInternalVar->setDisabled(true);
    ui->btnEditInputVar->setDisabled(true);
    ui->btnEditOutputVar->setDisabled(true);
    ui->btnEditInternalVar->setDisabled(true);
}

VariablesDisplay::~VariablesDisplay()
{
    delete ui;
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

void VariablesDisplay::insertVariable(QLabel *type){
    QString name = "name";
    QString value = "value";
    QFormLayout *layout = qobject_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());

    QLabel *newLblName = new QLabel(name, ui->scrollAreaWidgetContents);
    QLabel *newLblValue = new QLabel(value, ui->scrollAreaWidgetContents);

    int row;
    layout->getWidgetPosition(type, &row, nullptr);
    layout->insertRow(row + 1, newLblName, newLblValue);

}

