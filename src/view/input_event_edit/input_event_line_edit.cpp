/**
 * Project name: ICP Project 2024/2025
 *
 * @file input_event_line_edit.cpp
 * @author xcervia00
 *
 * @brief Custom line edit with keypress handling
 *
 */

#include "view/input_event_edit/input_event_line_edit.h"

#include <QLineEdit>
#include <QKeyEvent>

InputEventLineEdit::InputEventLineEdit(QWidget *parent) : QLineEdit(parent)
{}

void InputEventLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(!event) return;

    switch(event->key())
    {
        //              ^
        //       Or     |
        // --->         |
        case Qt::Key_Up:
        case Qt::Key_Right:
            emit upArrowPressed();
            break;

        //              |
        //       Or     |
        // <---         V
        case Qt::Key_Down:
        case Qt::Key_Left:
            emit downArrowPressed();
            break;
        default:
            break;
    }

    QLineEdit::keyPressEvent(event);
}