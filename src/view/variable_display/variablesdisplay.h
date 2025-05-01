/**
 * Project name: ICP Project 2024/2025
 *
 * @file variabledisplay.h
 * @author  xkadlet00
 *
 * @brief Defines the interface of display of input/output/internal variables
 *
 */

#ifndef VARIABLESDISPLAY_H
#define VARIABLESDISPLAY_H

#include "qlabel.h"
#include "qpushbutton.h"
#include "view/internal_representations.h"
#include <QWidget>

namespace Ui {
class VariablesDisplay;
}

class VariablesDisplay : public QWidget
{
    Q_OBJECT

public:

    explicit VariablesDisplay(QWidget *parent = nullptr);
    ~VariablesDisplay();
    void hideOrShow();
    /**
     * @brief inserts a variable
     * @param type under which label the new variable belongs
     * @return returns a variable that was added (in Qt representation)
     */
    FSMVariable insertVariable(enum variableType type, QString name, QString value);

    /**
     * @brief gets all necessary info for variables
     * @param type under which type the new variable belongs
     */
    void getVariableInfoInsert(enum variableType type);

    /**
     * @brief gets all necessary info for variables
     * @param type under which label the deleting variable belongs
     */
    void getVariableInfoDelete(enum variableType type);

    /**
     * @brief gets all necessary info for variables
     * @param type under which label the edited variable belongs
     */
    void getVariableInfoEdit(enum variableType type);

    /**
     * @brief activates/deactivates buttons (delete and edit)
     * @param b true or false based on whether we are acitvating or desactivating them
     * @param type what type input/output/internal i am acitvating it for
     */
    void setActButtons(bool b, enum variableType type);

signals:
    /**
     * @brief user wants to add a variable
     * @param type if its internal/input/output
     */
    void addVariableToDisplay(enum variableType type);

    /**
     * @brief user wants to remove a variable
     * @param type if its internal/input/output
     */
    void removeVariableFromDisplay(enum variableType type);

    /**
     * @brief user wants to edit a variable
     * @param type if its internal/input/output
     */
    void editVariableInDisplay(enum variableType type);

private:


    Ui::VariablesDisplay *ui;
    bool shown = true;
    QLabel *typeVar[3];
};

#endif // VARIABLESDISPLAY_H
