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
#include <QHash>
#include "statefsmwidget.h"
#include "workarea.h"
#include <vector>
#include <memory>
#include <QCloseEvent>
#include "variablesdisplay.h"

#include "mvc_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class EditorWindow;
}
QT_END_NAMESPACE

class EditorWindow: public QMainWindow, public FsmInterface
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
protected:
    /**
     * @brief overrides the default closeEvent -- asks if saving is wanted
     * @param event that happened
     */
    void closeEvent(QCloseEvent *event) override;
private:
    /**
     * @brief checks wheter or not a FSM can be inserted in a provided spot
     * (checking for collision with other states and going out of bounds)
     * @param position position of FSM we want to check if fits
     * @return true if it would fit
     */
    bool checkIfFSMFits(QPoint position);

    /**
     * @brief returns the minimum size the work-area can be based on position of FSM states
     * @return minimum size
     */
    QPoint getMinWorkAreaSize();

    /**
     * @brief resizes work-area
     */
    void resizeWorkArea();


    // ========================
    //       MVC INTERFACE 
    // ========================

    void updateState(const QString &name, const QPoint &pos) override;
    void updateStateName(const QString &oldName, const QString &newName) override;
    void updateAction(const QString &parentState, const QString &action) override;
    void updateActiveState(const QString &name) override;

    void updateCondition(size_t transitionId, const QString &condition) override;
    void updateTransition(size_t transitionId, const QString &srcState, const QString &destState) override;
    void updateVarInput(const QString &name, const QString &value) override;
    void updateVarOutput(const QString &name, const QString &value) override;
    void updateVarInternal(const QString &name, const QVariant &value) override;

    void destroyState(const QString &name) override;
    void destroyAction(const QString &parentState) override;
    void destroyCondition(size_t transitionId) override;
    void destroyTransition(size_t transitionId) override;
    void destroyVarInput(const QString &name) override;
    void destroyVarOutput(const QString &name) override;
    void destroyVarInternal(const QString &name) override;

    void loadFile(const QString &filename) override;
    void saveFile(const QString &filename) override;

    void renameFsm(const QString &name) override;

    // This may be used only one-way
    void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const override;
    void log() const override; // Request log

    void startInterpretation() override;
    void stopInterpretation() override;

    void cleanup() override;
    void throwError(FsmErrorType errNum) override;
    void throwError(FsmErrorType errNum, const QString &errMsg) override;

    void outputEvent(const QString &outName) override;
    void inputEvent(const QString &name, const QString &value) override;

    // MVC Related, but specific to view 
    void registerModel(std::shared_ptr<FsmInterface> model);


    // ========================
    //       Attributes 
    // ========================
    Ui::EditorWindow *ui;
    QLabel * statusBarLabel;//label on status bar
    WorkArea * workArea;// work area widget
    QWidget * workAreaScrollContainer;// container for scroll area
    QLayout * workAreaScrollLayout; // layout for scroll area
    std::vector<StateFSMWidget*> allStates; //vector of all states in the system
    VariablesDisplay * variablesDisplay;

    std::shared_ptr<FsmInterface> model; ///< Reference to model

    // Tady je doporučení, jak to může fungovat, není to závazné (použij místo QString/QVariant ten typ, co potřebuješ. 
    // Ale ten první typ pro indexaci by měla být podle QStringu)
    //QHash<QString,QVariant> varsInternal; ///< Internal variable - may be of variable type
    //QHash<QString,QString> varsInput; ///< Input variable - only string format
    //QHash<QString,QString> varsOutput; ///< Output variable - only string format
    // QHash<QString,StateFSMWidget*> states; ///< List of all states used within the FSM
    // QHash<size_t,...*> transitions; // Nevim co za typ pouzijes pro toto
};
#endif // EDITORWINDOW_H
