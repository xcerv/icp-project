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
    explicit VariablesDisplay(QWidget *parent = nullptr);
    ~VariablesDisplay();
    void hideOrShow();
    /**
     * @brief inserts a variable
     * @param under which label the new variable belongs
     */
    void insertVariable(QLabel *type);

private:
    struct FSMVariable {
        QPushButton *name;
        QLabel *value;
    };

    Ui::VariablesDisplay *ui;
    bool shown = true;

    QVector<FSMVariable> inputVar;
    QVector<FSMVariable> outputVar;
    QVector<FSMVariable> internalVar;
};

#endif // VARIABLESDISPLAY_H
