/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow.h
 * @author  xkadlet00
 *
 * @brief window where FSM is edited
 *
 */

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class EditorWindow;
}
QT_END_NAMESPACE

class EditorWindow: public QMainWindow
{
    Q_OBJECT

public:
    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();
    void resizeWorkArea(int width, int height);

private:
    Ui::EditorWindow *ui;
};
#endif // EDITORWINDOW_H
