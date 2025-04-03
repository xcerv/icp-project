#include "statefsmwidget.h"
#include "ui_statefsmwidget.h"
#include <QMouseEvent>

StateFSMWidget::StateFSMWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StateFSMWidget)
{
    ui->setupUi(this);
    setFixedSize(150,180);
}

StateFSMWidget::~StateFSMWidget()
{
    delete ui;
}

void StateFSMWidget::changeStateName(){
    ui->name->setText("lol");
}

void StateFSMWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit rightClick();
    }
}
