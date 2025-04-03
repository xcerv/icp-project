/**
 * Project name: ICP Project 2024/2025
 *
 * @file statefsmwidget.cpp
 * @author  xkadlet00
 *
 * @brief defines the structure of a FSM state
 *
 */

#include "statefsmwidget.h"
#include "ui_statefsmwidget.h"
#include <QMouseEvent>

StateFSMWidget::StateFSMWidget(QPoint pos, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StateFSMWidget)
{
    ui->setupUi(this);
    size.setX(150);
    size.setY(180);
    setFixedSize(150,180);

    position = pos;
    scrollContainer = new QWidget;
    scrollLayout = new QVBoxLayout(scrollContainer);
    scrollContainer->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollContainer);  // add container to scroll area
}

StateFSMWidget::~StateFSMWidget()
{
    for (QLabel* label : allConditions) {
        delete label;  // free all condition labels
    }
    allConditions.clear();  // ensure the vector is empty
    delete scrollContainer;
    delete scrollLayout;
    delete ui;
}



void StateFSMWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit rightClick();
    }
}

void StateFSMWidget::setName(QString name){
    ui->name->setText(name);
}


void StateFSMWidget::addCondition(QString cond){
    QLabel * l = new QLabel;
    l->setText(cond);
    scrollLayout->addWidget(l);
    allConditions.push_back(l);
}

QPoint StateFSMWidget::getSize(){
    return size;
}

QPoint StateFSMWidget::getPosition(){
    return position;
}
