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

    size.setX(160);
    size.setY(200);
    setFixedSize(160,200);


    position = pos;

    ui->output->setPlainText("");
    ui->output->setReadOnly(true);
    ui->output->setContextMenuPolicy(Qt::NoContextMenu);
    ui->output->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    ui->output->viewport()->setCursor(Qt::ArrowCursor);
    ui->output->setFocusPolicy(Qt::NoFocus);
    ui->output->viewport()->installEventFilter(this);

}

StateFSMWidget::~StateFSMWidget()
{
    delete ui;
}

void StateFSMWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClick();
    } else if (event->button() == Qt::RightButton) {
        emit rightClick();
    }
}

bool StateFSMWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->output->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            emit leftClick();
            return true;
        } else if (mouseEvent->button() == Qt::RightButton) {
            QWidget *child = ui->output->childAt(mouseEvent->pos());
            if (child && child->inherits("QScrollBar")) {
                return false; // let scrollbar handle the event
            }
            emit rightClick();
            return true;
        }
    }
    // pass unhandled events on
    return QWidget::eventFilter(obj, event);
}


void StateFSMWidget::setName(QString name){
    ui->name->setText(name);
}

QString StateFSMWidget::getName(){
    return ui->name->text();
}
void StateFSMWidget::setOutput(QString cond){
    ui->output->setPlainText(cond);
}

QString StateFSMWidget::getOutput(){
    return ui->output->toPlainText();
}

QPoint StateFSMWidget::getSize(){
    return size;
}

QPoint StateFSMWidget::getPosition(){
    return position;
}

void StateFSMWidget::setPosition(QPoint pos){
    if (position != pos){
        position = pos;
        move(pos);
    } 
}

void StateFSMWidget::recolor(const QString& c1, const QString& c2, const QString& ar) {
    QString style = QString(
                        "QWidget { background: %1; }"
                        "QTextEdit { color: %2; font: 14px \"Nimbus Mono PS\"; "
                        "   padding: 5px 5px 5px 5px;}"
                        "QLabel { color: %2; font: 14px \"Nimbus Mono PS\"; }"
                        "QScrollArea { border: 5px solid %1; }"
                        "QScrollArea::corner { background: transparent; }"

                        "QScrollBar::vertical"
                        "{background-color:  lightgray;width: 15px;margin: 15px 3px 15px 3px;border:0px;}"
                        "QScrollBar::horizontal"
                        "{background-color:  lightgray;height: 15px;margin: 3px 15px 3px 15px;border:0px;}"

                        "QScrollBar::handle{background-color: %2;min-height: 5px;}"

                        "QScrollBar::sub-line:vertical"
                        "{margin: 0px 0px 3px 0px;border-image: url(:/arrows/img/up%3.svg);"
                        "border-width:0px;background-color:transparent;height: 10px;"
                        "width: 9px;subcontrol-position: top;subcontrol-origin: margin;}"

                        "QScrollBar::add-line:vertical"
                        "{margin: 3px 0px 0px 0px;border-image: url(:/arrows/img/down%3.svg);"
                        "border-width:0px;background-color:transparent;height: 10px;"
                        "width: 9px;subcontrol-position: bottom;subcontrol-origin: margin;}"

                        "QScrollBar::sub-line:horizontal"
                        "{margin: 0px 3px 0px 0px;border-image: url(:/arrows/img/left%3.svg);"
                        "border-width:0px;background-color:transparent;height: 9px;"
                        "width: 10px;subcontrol-position: left;subcontrol-origin: margin;}"

                        "QScrollBar::add-line:horizontal"
                        "{margin: 0px 0px 0px 3px;border-image: url(:/arrows/img/right%3.svg);"
                        "border-width:0px;background-color:transparent;height: 9px;"
                        "width: 10px;subcontrol-position: right;subcontrol-origin: margin;}"

                        "QTextEdit, QListView {background-attachment: scroll;}"
                        ).arg(c1, c2, ar);
    setStyleSheet(style);
}
