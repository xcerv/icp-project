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

/**
 * @brief Modification of line edit to detect other button presses (arrow keys)
 */
class InputEventLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for line edit
     * @param parent Parent widget
     */
    explicit InputEventLineEdit(QWidget *parent = nullptr);

protected:
    /**
     * @brief Key on keyboard was pressed
     * @param event The event to handle
     */
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void upArrowPressed();
    void downArrowPressed();
};

#endif // MYCUSTOMQLINEEDIT_H