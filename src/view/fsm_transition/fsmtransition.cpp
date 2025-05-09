/**
 * Project name: ICP Project 2024/2025
 *
 * @file fsmtransition.cpp
 * @author  xkadlet00
 * @author  xcervia00
 *
 * @brief Transition definition
 *
 */

#include "fsmtransition.h"
#include <QPainter>

#define BREATHINGROOM 50
#define LIFTLOOP 30
#define SQUEEZE 20

FSMTransition::FSMTransition(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    editTransitionBtn = new QPushButton("⚙️", parent);
    editTransitionBtn->raise();
    editTransitionBtn->show();
    btnSize = QPoint(20, 20);
    editTransitionBtn->setFixedSize(btnSize.x(),btnSize.y());
    editTransitionBtn->setStyleSheet("QPushButton{"
                                     "  border-width: 1px;"
                                     "  border-color: black;"
                                     "  border-style: solid;"
                                     "  background-color: white;"
                                     "  color: navy;"
                                     "  border-radius: 10px;"
                                     "}"
                                     "QPushButton::hover{"
                                     "  background-color: navy;"
                                     "  color: white"
                                     "}");

    connect(editTransitionBtn, &QPushButton::clicked, this, &FSMTransition::clickedEdit);
}


FSMTransition::~FSMTransition(){
    delete editTransitionBtn;
}

void FSMTransition::paintEvent(QPaintEvent *event){
    QPainter p(this);
    Q_UNUSED(event);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 2);
    if(!isLoop){
        p.drawLine(startPos,finPos);
    }else{
        QPoint start = startPos;
        QPoint end = finPos;

        QPoint helpPos = start;
        QPoint helpPos2 = end;

        if (start.y() - BREATHINGROOM > 0) {
            // Go up
            end.setY(start.y());
            helpPos2 = end;
            helpPos.setY(start.y() - LIFTLOOP);
            helpPos2.setY(end.y() - LIFTLOOP);
        } else {
            // Go down
            start.setY(end.y());
            helpPos = start;
            helpPos.setY(start.y() + LIFTLOOP);
            helpPos2.setY(end.y() + LIFTLOOP);
        }

        p.drawLine(start, helpPos);
        p.drawLine(helpPos, helpPos2);
        p.drawLine(helpPos2, end);

    }
    editTransitionBtn->raise();
}

void FSMTransition::relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize){
    startSize += startPoint;
    finSize += finPoint;

    if(startPoint == finPoint){
        isLoop = true;
        startPos = startPoint;
        startPos.setX(startPos.x() + SQUEEZE);
        finPos = startSize;
        finPos.setX(finPos.x() - SQUEEZE);
    }else{
        if(startSize.y() >= finPoint.y() && startPoint.y() <= finSize.y()){
            if(startPoint.x() < finSize.x()){
                //  [ ]
                //  [ ]-->[ ]
                //        [ ]
                startPos.setY(((startSize.y() - finPoint.y())/2) + finPoint.y());
                startPos.setX(startSize.x());
                finPos.setX(finPoint.x());
            }else{
                //  [ ]
                //  [ ]<--[ ]
                //        [ ]
                startPos.setY((finSize.y()-startPoint.y())/2 + startPoint.y());
                startPos.setX(startPoint.x());
                finPos.setX(finSize.x());
            }
            finPos.setY(startPos.y());
        }else if(finPoint.x() <= startSize.x() && finSize.x() >= startPoint.x()){
            if(startSize.y() <= finPoint.y()){
                // [ ]
                //  |
                //  V
                // [ ]
                startPos.setY(startSize.y());
                finPos.setY(finPoint.y());
            }else{
                // [ ]
                //  A
                //  |
                // [ ]
                startPos.setY(startPoint.y());
                finPos.setY(finSize.y());
            }
            int xLeft = std::min(finPoint.x(),startPoint.x());
            int xRight = std::max(finSize.x(),startSize.x());
            startPos.setX(((xRight-xLeft)/2) + xLeft);
            finPos.setX(startPos.x());
        }else if(startSize.x() < finPoint.x() && startSize.y() < finPoint.y()){
            // [ ]
            //    \
            //     V
            //     [ ]
            startPos.setY(startSize.y());
            startPos.setX(startSize.x());
            finPos.setY(finPoint.y());
            finPos.setX(finPoint.x());
        }else if(startPoint.x() > finSize.x() && startSize.y() < finPoint.y()){
            //     [ ]
            //    /
            //   V
            //[ ]
            startPos.setX(startPoint.x());
            startPos.setY(startSize.y());
            finPos.setX(finSize.x());
            finPos.setY(finPoint.y());
        }else if(startPoint.x() > finSize.x() && startPoint.y() > finSize.y()){
            // [ ]
            //    A
            //     \
            //     [ ]
            startPos = startPoint;
            finPos = finSize;
        }else{
            //     [ ]
            //    A
            //   /
            //[ ]
            startPos.setX(startSize.x());
            startPos.setY(startPoint.y());
            finPos.setX(finPoint.x());
            finPos.setY(finSize.y());
        }
    }


    // Find a bounding rect between start and end to cover the full area
    QPoint st;
    st.setX(0);
    st.setY(0);
    QPoint fn;
    fn.setX(finPos.x() > startPos.x()? finPos.x():startPos.x());
    fn.setY(finPos.y() > startPos.y()? finPos.y():startPos.y());

    QRect bounds = QRect(st, fn).normalized().adjusted(0,0,0,BREATHINGROOM); // with padding
    setGeometry(bounds);
    QPoint center = (startPos + finPos) / 2;
    if(isLoop){
        if(startPoint.y() > BREATHINGROOM){
            center.setY(startPoint.y() - LIFTLOOP);
        }else{
            center.setY(startSize.y() + LIFTLOOP);
        }
    }
    center -= (btnSize/2);
    editTransitionBtn->move(center);
    raise();
    show();
    setVisible(true);
    update();
}



void FSMTransition::clickedEdit(){
    emit editTransition(this);
}


void FSMTransition::addTransition(size_t num){
    individualTransitions.insert(num);
}
void FSMTransition::subTransition(size_t num){
    individualTransitions.remove(num);
}
QSet<size_t> FSMTransition::getTransitions(){
    return individualTransitions;
}
