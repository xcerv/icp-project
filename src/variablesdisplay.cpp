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

void VariablesDisplay::insertVariable(enum variableType type, QString name, QString value){
    QFormLayout *layout = qobject_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());

    QLabel *newLblName = new QLabel(name, ui->scrollAreaWidgetContents);
    QLabel *newLblValue = new QLabel(value, ui->scrollAreaWidgetContents);

    int row;
    layout->getWidgetPosition(typeVar[type], &row, nullptr);
    layout->insertRow(row + 1, newLblName, newLblValue);
    FSMVariable h{newLblName,newLblValue};
    allVars[type].append(h);
}


void VariablesDisplay::getVariableInfoInsert(enum variableType type){
    // make dialog for getting neccassary info
    QDialog dialog(this);
    dialog.setStyleSheet("QWidget{background-color: white}");
    dialog.setWindowTitle("Resize work-area");

    QFormLayout form(&dialog);

    // two spinboxes (width, height)
    QLineEdit *nameInput = new QLineEdit(&dialog);
    form.addRow("Name:", nameInput);

    // OK + Cancel buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(type == OUTPUTV){
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "") {
            insertVariable(type,nameInput->text(),"");
        }
    }else{
        QLineEdit *valueInput = new QLineEdit(&dialog);
        form.addRow("Value:", valueInput);
        form.addRow(&buttonBox);
        if (dialog.exec() == QDialog::Accepted && nameInput->text() != "" && valueInput->text() != "") {
            insertVariable(type,nameInput->text(),valueInput->text());
        }
    }
}
