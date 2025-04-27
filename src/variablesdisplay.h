#ifndef VARIABLESDISPLAY_H
#define VARIABLESDISPLAY_H

#include "qlabel.h"
#include "qpushbutton.h"
#include <QWidget>

namespace Ui {
class VariablesDisplay;
}

class VariablesDisplay : public QWidget
{
    Q_OBJECT

public:

    enum variableType{
        INPUTV,
        OUTPUTV,
        INTERNALV,
        NUMV
    };

    explicit VariablesDisplay(QWidget *parent = nullptr);
    ~VariablesDisplay();
    void hideOrShow();
    /**
     * @brief inserts a variable
     * @param type under which label the new variable belongs
     */
    void insertVariable(enum variableType type, QString name, QString value);

    /**
     * @brief gets all necessary info for variables
     * @param type under which label the new variable belongs
     */
    void getVariableInfoInsert(enum variableType type);

private:
    struct FSMVariable {
        QLabel *name;
        QLabel *value;
    };

    Ui::VariablesDisplay *ui;
    bool shown = true;

    QVector<FSMVariable> allVars[3];
    QLabel *typeVar[3];
};

#endif // VARIABLESDISPLAY_H
