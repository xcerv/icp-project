#ifndef VARIABLESDISPLAY_H
#define VARIABLESDISPLAY_H

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

private:
    Ui::VariablesDisplay *ui;
    bool shown = true;
};

#endif // VARIABLESDISPLAY_H
