/**
 * Project name: ICP Project 2024/2025
 *
 * @file variablesdisplay.h
 * @author  xkadlet00
 *
 * @brief Defines the interface of display of input/output/internal variables
 *
 */

#ifndef VARIABLESDISPLAY_H
#define VARIABLESDISPLAY_H

#include "qlabel.h"
#include "view/internal_representations.h"
#include <QWidget>

namespace Ui {
class VariablesDisplay;
}

/**
 * @brief a widget that displays variables
 */
class VariablesDisplay : public QWidget
{
    Q_OBJECT

public:

    explicit VariablesDisplay(QWidget *parent = nullptr);
    virtual ~VariablesDisplay();


    /**
     * @brief Toggles visibility of the window
     */
    void hideOrShow();

    /**
     * @brief Explicitly sets visibility of variableDisplay
     * @param visibility Shows if true, hides if false
     */
    void setDisplayVisibility(bool visibility);
    
    /**
     * @brief inserts a variable
     * @param type under which label the new variable belongs
     * @param name Name of the variable
     * @param value Value of the variable
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
     * @param b true or false based on whether we are activating or deactivating them
     * @param type what type input/output/internal i am activating it for
     */
    void setActButtons(bool b, enum variableType type);

    /**
     * @brief activates/deactivates *ALL* buttons (delete and edit and add)
     * @param activate true or false based on whether we are activating or deactivating them
     */
    void setActButtonsAll(bool activate);

    /**
     * @brief activates/deactivates buttons (delete and edit), but enables them for adding new
     * @param activate true or false based on whether we are activating or deactivating them
     * @param type what type input/output/internal i am activating it for
     */
    void setActButtonsAdding(bool activate, variableType type);

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


    Ui::VariablesDisplay *ui; ///< The ui itself
    bool shown = true; ///< Is the variable dispaly shown
    QLabel *typeVar[3]; ///< Labels of the variables on the menu
};

#endif // VARIABLESDISPLAY_H
