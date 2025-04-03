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
#include "statefsmwidget.h"
#include "workarea.h"
#include <vector>

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

    /**
     * @brief resizes workArea
     * @param width
     * @param height
     * @todo check if no states "fall out"
     */
    void resizeWorkArea(int width, int height);

    /**
     * @brief adds new FSM state into workArea if possible
     * @param coords
     * @param name
     * @todo check if doesnt fall out of workArea + doesnt collide with other states
     */
    void insertFSMState(QPoint position, QString name);

public slots:
    /**
     * @brief what happens after workArea is left clicked (no action)
     * @param position
     */
    void workAreaLeftClick(QPoint position);
    /**
     * @brief what happens after workArea is right clicked (now spawns new state)
     * @param position
     * @todo bring up menu with options to do (add state, resize workArea, .. )
     */
    void workAreaRightClick(QPoint position);
    /**
     * @brief after any FSM is right clicked
     * @todo bring up menu with possible actions (delete, copy, add condition, connect, set as first state, .. )
     */
    void stateFSMRightClick();
    /**
     * @brief after any FSM is left clicked
     * @todo should work only for connecting, maybe opening detailed info about state??
     */
    void stateFSMLeftClick();
private:
    Ui::EditorWindow *ui;

    QLabel * statusBarLabel;//label on status bar
    WorkArea * workArea;// work area widget
    QWidget * workAreaScrollContainer;// container for scroll area
    QLayout * workAreaScrollLayout; // layout for scroll area
    std::vector<StateFSMWidget*> allStates; //vector of all states in the system
};
#endif // EDITORWINDOW_H
