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
    if (event->key() == Qt::Key_Up) {
        emit upArrowPressed();
        QLineEdit::keyPressEvent(event);
    } else if (event->key() == Qt::Key_Down) {
        emit downArrowPressed();
        QLineEdit::keyPressEvent(event);
    }

    if (!(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) || !event->isAccepted()) {
        QLineEdit::keyPressEvent(event);
    }
}