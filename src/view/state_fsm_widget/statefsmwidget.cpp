/**
 * Project name: ICP Project 2024/2025
 *
 * @file statefsmwidget.cpp
 * @author  xkadlet00
 *
 * @brief defines the structure of a FSM state
 *
 */

#include "view/state_fsm_widget/statefsmwidget.h"
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
    output = new QLabel;
    output->setText("");
    output->setWordWrap(true);
    scrollLayout->addWidget(output);
}

StateFSMWidget::~StateFSMWidget()
{
    /*
    for (QLabel* label : allOutputs) {
        delete label;  // free all output labels
    }
    allOutputs.clear();  // ensure the vector is empty
    delete scrollContainer;
    delete scrollLayout;
    */
    delete ui;
}



void StateFSMWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClick();
    } else if (event->button() == Qt::RightButton) {
        emit rightClick();
    }
}

void StateFSMWidget::setName(QString name){
    ui->name->setText(name);
}

QString StateFSMWidget::getName(){
    return ui->name->text();
}
void StateFSMWidget::setOutput(QString cond){
    output->setText(cond);
}

QString StateFSMWidget::getOutput(){
    return output->text();
}

QPoint StateFSMWidget::getSize(){
    return size;
}

QPoint StateFSMWidget::getPosition(){
    return position;
}

void StateFSMWidget::setPosition(QPoint pos){
    position = pos;
    move(pos);
}

void StateFSMWidget::recolor(const QString& c1, const QString& c2) {
    QString style = QString(
                        "QWidget { background: %1; }"
                        "QLabel { color: %2; font: 14px \"Nimbus Mono PS\"; }"
                        "QScrollArea { border: 5px solid %1; }"
                        "QScrollArea::corner { background: transparent; }"

                        "QScrollBar::vertical"
                        "{background-color:  lightgray;width: 15px;margin: 15px 3px 15px 3px;border:0px;}"
                        "QScrollBar::horizontal"
                        "{background-color:  lightgray;height: 15px;margin: 3px 15px 3px 15px;border:0px;}"

                        "QScrollBar::handle{background-color: %2;min-height: 5px;}"

                        "QScrollBar::sub-line:vertical"
                        "{margin: 0px 0px 3px 0px;border-image: url(:/arrows/img/up.svg);"
                        "border-width:0px;background-color:transparent;height: 10px;"
                        "width: 9px;subcontrol-position: top;subcontrol-origin: margin;}"

                        "QScrollBar::add-line:vertical"
                        "{margin: 3px 0px 0px 0px;border-image: url(:/arrows/img/down.svg);"
                        "border-width:0px;background-color:transparent;height: 10px;"
                        "width: 9px;subcontrol-position: bottom;subcontrol-origin: margin;}"

                        "QScrollBar::sub-line:horizontal"
                        "{margin: 0px 3px 0px 0px;border-image: url(:/arrows/img/left.svg);"
                        "border-width:0px;background-color:transparent;height: 9px;"
                        "width: 10px;subcontrol-position: left;subcontrol-origin: margin;}"

                        "QScrollBar::add-line:horizontal"
                        "{margin: 0px 0px 0px 3px;border-image: url(:/arrows/img/right.svg);"
                        "border-width:0px;background-color:transparent;height: 9px;"
                        "width: 10px;subcontrol-position: right;subcontrol-origin: margin;}"

                        "QTextEdit, QListView {background-attachment: scroll;}"
                        ).arg(c1, c2);

    setStyleSheet(style);
}
