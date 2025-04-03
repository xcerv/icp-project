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
#include <QLabel>
#include "workarea.h"

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

public slots:
    void workAreaLeftClick(QPoint position);
    void workAreaRightClick(QPoint position);
    void stateFSMRightClick();
private:
    Ui::EditorWindow *ui;

    QLabel * statusBarLabel;
    WorkArea * workArea;
    QWidget * workAreaScrollContainer;
    QLayout * workAreaScrollLayout;
};
#endif // EDITORWINDOW_H
