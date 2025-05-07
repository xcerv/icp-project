/**
 * Project name: ICP Project 2024/2025
 *
 * @file input_event_line_edit.h
 * @author xcervia00
 *
 * @brief Custom line edit with keypress handling
 *
 */

#ifndef INPUT_EVENT_LINE_EDIT_H_
#define INPUT_EVENT_LINE_EDIT_H_

#include <QLineEdit>
#include <QKeyEvent>

class InputEventLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit InputEventLineEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void upArrowPressed();
    void downArrowPressed();
};

#endif // MYCUSTOMQLINEEDIT_H