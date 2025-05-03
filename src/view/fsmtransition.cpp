#include "fsmtransition.h"
#include <QPainter>

FSMTransition::FSMTransition(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    editTransitionBtn = new QPushButton("Edit", parent);
    editTransitionBtn->raise();
    editTransitionBtn->show();
    btnSize = QPoint(40, 20);
    editTransitionBtn->setFixedSize(btnSize.x(),btnSize.y());
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
    p.drawLine(startPos,finPos);
    editTransitionBtn->raise();
}

void FSMTransition::relocateTransition(QPoint startPoint, QPoint startSize, QPoint finPoint, QPoint finSize){
    startSize += startPoint;
    finSize += finPoint;
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
    }else if(true){
        //     [ ]
        //    A
        //   /
        //[ ]
        startPos.setX(startSize.x());
        startPos.setY(startPoint.y());
        finPos.setX(finPoint.x());
        finPos.setY(finSize.y());
    }


    // Find a bounding rect between start and end to cover the full area
    QPoint st;
    st.setX(0);
    st.setY(0);
    QPoint fn;
    fn.setX(finPos.x() > startPos.x()? finPos.x():startPos.x());
    fn.setY(finPos.y() > startPos.y()? finPos.y():startPos.y());

    //QRect bounds = QRect(st, fn).normalized().adjusted(-10, -10, 10, 10); // with padding
    QRect bounds = QRect(st, fn).normalized(); // with padding
    setGeometry(bounds);
    QPoint center = (startPos + finPos) / 2;
    center -= (btnSize/2);
    editTransitionBtn->move(center);
    raise();
    show();
    setVisible(true);
    update();
}


void FSMTransition::setDst(QString ndst){
    dst = ndst;
}

void FSMTransition::setSrc(QString nsrc){
    src = nsrc;
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
