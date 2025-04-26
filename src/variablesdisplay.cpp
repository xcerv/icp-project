#include "variablesdisplay.h"
#include "ui_variablesdisplay.h"

VariablesDisplay::VariablesDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VariablesDisplay)
{
    ui->setupUi(this);
    connect(ui->btnHide, &QPushButton::clicked,this, &VariablesDisplay::hideOrShow);
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

    ui->lblInputVar->setVisible(shown);
    ui->lblOutputVar->setVisible(shown);
    ui->lblInternalVar->setVisible(shown);

    ui->btnAddInputVar->setVisible(shown);
    ui->btnAddOutputVar->setVisible(shown);
    ui->btnAddInternalVar->setVisible(shown);
    ui->scrollArea->setVisible(shown);
}


