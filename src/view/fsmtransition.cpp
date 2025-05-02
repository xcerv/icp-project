#include "fsmtransition.h"
#include <QPainter>

FSMTransition::FSMTransition(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
}


void FSMTransition::paintEvent(QPaintEvent *event){
    QPainter p(this);
    Q_UNUSED(event);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 2);
    QPoint start, finish;
    start.setX(100);
    start.setY(100);
    finish.setX(300);
    finish.setY(300);
    p.drawLine(start,finish);
}


void FSMTransition::relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize){
    startSize += startPoint;
    finSize += finPoint;
    /*
    if(startPoint.x() < finSize.x()){ //fin state is to the right of start state
        if(startSize.y() > finPoint.y()){
            //  [ ]  [ ]

        }
    }else{

    }*/
    update();
}

