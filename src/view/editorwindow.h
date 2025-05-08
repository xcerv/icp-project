/**
 * Project name: ICP Project 2024/2025
 *
 * @file editorwindow.h
 * @author  xkadlet00
 * @author  xcervia00
 *
 * @brief window where FSM is edited
 *
 */

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHash>
#include <memory>
#include <QCloseEvent>
#include <QTextEdit>
#include <QComboBox>
#include <QPlainTextEdit>

#include "view/work_area/workarea.h"
#include "view/input_event_edit/input_event_line_edit.h"
#include "view/state_fsm_widget/statefsmwidget.h"
#include "view/variable_display/variablesdisplay.h"
#include "view/logging_window/loggingwindow.h"
#include "view/internal_representations.h"
#include "view/fsmtransition.h"

#include "mvc_interface.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class EditorWindow;
}
QT_END_NAMESPACE

/**
 * @brief 
 */
class EditorWindow: public QMainWindow, public FsmInterface
{
    Q_OBJECT

public:

    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

    
    /**
     * @brief Related to MVC interface communication; registers a model to use
     * @param model The model to use 
     */
    void registerModel(FsmInterface *model);

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

private slots:
    void workAreaMouseMoved(QPoint pos);

public slots:


    /* Interpreter UI elements */
    /**
     * @brief Action triggered when interpretation stop button is clicked
     */
    void stopButtonClick();
    /**
     * @brief Action triggered when interpretation start button is clicked
     */
    void startButtonClick();
    /**
     * @brief Action triggered when interpretation start button is clicked
     */
    void submitInputClick();

    /**
     * @brief Event fired when ComboxValue had changed
     */
    void inputComboxChanged();

    /* Save-loading Related */

    /**
     * @brief Handler for saving files
     */
    void handleActionSaveAs();
    /**
     * @brief Save current session
     */
    void handleActionSave();
    /**
     * @brief Cleanup workarea and load blank session
     */
    void handleActionNew();
    /**
     * @brief Load fsm from file
     */
    void handleActionLoad();
    /**
     * @brief What to do when Exit function is triggered
     * @note This is different to standard closing; This is triggered by the Exit action
     */
    void handleActionExit();
    /**
     * @brief Prints help message
     */
    void handleActionHelp();
    /**
     * @brief Prints info about the program
     */
    void handleActionAbout();
    /**
     * @brief Prints info about hotkeys
     */
    void handleActionHotkeys();
    /**
     * @brief Renames FSM
     */
    void handleActionRenameFsm();
    /**
     * @brief Resizes the work area
     */
    void handleActionResize();
    /**
     * @brief Renames a state
     */
    void handleActionRenameState(StateFSMWidget* state);
    /**
     * @brief Edits a state action
     */
    void handleActionEditState(StateFSMWidget* state);
    /**
     * @brief Makes a state active/inintial
     */
    void handleActionActiveState(StateFSMWidget* state);
    /**
     * @brief Deletes a hover-over state
     */
    void handleActionDeleteState(StateFSMWidget* state);
    /**
     * @brief Inserts a new state
     * @param position Position of the state
     */
    void handleActionAddState(QPoint position);

    /**
     * @brief Handler for moving a state
     * @param position 
     */
    void handleActionConnectState(StateFSMWidget* state);
    /**
     * @brief Handler for moving a state
     * @param position 
     */
    void handleActionMoveState(StateFSMWidget* movingState);

    /**
     * @brief Cancels movement of a state
     */
    void cancelActionMove();

    /**
     * @brief Cancels connecting action of a state
     */
    void cancelActionConnect();

    /**
     * @brief Dynamically update transitions when moving an object
     */
    void movementUpdateTransitions();

    /**
     * @brief Returns the StateFSMWidget that is pointed at by the mouse
     * @return Pointer to the FSM widget or nullptr if nothing is found
     */
    StateFSMWidget* getHoveredState();

    /* Work Area UI Elements */
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
    /**
     * @brief User wants to add a variable
     * @param type of variable to be added
     */
    void variableToBeAdded(enum variableType type);

    /**
     * @brief User wants to delete a variable
     * @param type of variable to be deleted
     */
    void variableToBeDeleted(enum variableType type);

    /**
     * @brief User wants to edit a variable
     * @param type of variable to be edited
     */
    void variableToBeEdited(enum variableType type);

    /**
     * @brief handles the signal of clicking on the edit button of a transition
     * @param transition the transition I just interacted with
     */
    void editTransitionHanling(FSMTransition * transition);
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
     * @param skip a state that can be skipped
     * @return true if it would fit
     */
    bool checkIfFSMFits(QPoint position, StateFSMWidget * skip = nullptr);

    /**
     * @brief execute a window for renaming
     * @param title title of the window
     * @return new name
     */
    QString renamingWindow(QString title);

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
    /**
     * @brief helper for more efficient updating
     * @param type
     * @param name
     * @param value
     */
    void updateVar(enum variableType type, const QString &name, const QString &value);

    void destroyState(const QString &name) override;
    void destroyAction(const QString &parentState) override;
    void destroyCondition(size_t transitionId) override;
    void destroyTransition(size_t transitionId) override;

    void destroyVarInput(const QString &name) override;
    void destroyVarOutput(const QString &name) override;
    void destroyVarInternal(const QString &name) override;
    /**
     * @brief helper for more efficient deleting
     * @param type
     * @param name
     */
    void destroyVar(enum variableType type, const QString &name);

    void loadFile(const QString &filename) override;
    void saveFile(const QString &filename) override;

    void renameFsm(const QString &name) override;

    // This may be used only one-way
    void log(const QString &time, const QString &state, const QString &varInputs, const QString &varOutputs, const QString &varInternals) const override;
    void log() const override; // Request log

    void startInterpretation() override;
    void stopInterpretation() override;
    void restoreInterpretationBackup() override;

    void cleanup() override;
    void throwError(FsmErrorType errNum) override;
    void throwError(FsmErrorType errNum, const QString &errMsg) override;

    void outputEvent(const QString &outName) override;
    void inputEvent(const QString &name, const QString &value) override;

    // ========================
    //       Hotkeys 
    // ========================

    /**
     * @brief Scrolls input combox up
     */
    void scrollInputComboxUp();
    /**
     * @brief Scrolls input combox down
     */
    void scrollInputComboxDown();

    // ========================
    //       Attributes 
    // ========================
    
    // UI elements
    Ui::EditorWindow *ui; ///< The ui itself
    QLabel * statusBarLabel = nullptr;///< label on status bar
    WorkArea * workArea = nullptr;///< work area widget
    QWidget * workAreaScrollContainer = nullptr;///< container for scroll area
    QLayout * workAreaScrollLayout = nullptr; ///< layout for scroll area
    VariablesDisplay * variablesDisplay = nullptr;/// Variable display
    LoggingWindow * loggingWindow = nullptr;///< Logging window

    // Interpreter buttons
    QPushButton * stopButton = nullptr; ///< Button for stopping interpreattion
    QPushButton * startButton = nullptr; ///< Button for starting interpretation
    QPushButton * inputSubmitButton = nullptr; ///< Button for submitting input

    InputEventLineEdit * inputEventField = nullptr; ///< Text field for inputting event value
    QComboBox * inputEventCombox = nullptr; ///< Combox for selecting which input to use
    QPlainTextEdit * outputEventField = nullptr; ///< The output of the FSM

    // File related
    bool fileModified = false; ///< Was the file modified since the last save?
    QString lastFileName; ///< The name of the last opened/saved file 

    // Error Handling
    bool isShowingError = false; ///< Is the FSM currently showing error message? 

    // State moving Helpers
    QString manipulatedState;///< A state that is being moved at the moment
    StateFSMWidget* movingStateWidget = nullptr;
    QLabel* ghostStateWidget = nullptr;
    QPoint movingStateOrigPos; ///< Original position of the moving state
    bool isStateMoving = false;///< wheter or not is any state moving

    // State Helpers
    StateFSMWidget * activeState = nullptr;///< A state that is active at the moment
    bool isStateConnecting = false;///< whether or not is any state connecting to another -- via transition

    bool isInterpreting = false;///< the system is running

    // Model-link
    FsmInterface* model = nullptr; ///< Reference to model
    
    // Entity storage
    QHash<QString,StateFSMWidget*> allStates; ///< List of all states used within the FSM
    QHash<QString, FSMVariable> allVars[3];///< representation of all variables used in FSM


    QHash<QPair<QString,QString>,FSMTransition *> allTransitionsUI;///< all transitions in UI identified by the two states it is between

    struct reprCondTr {
        QString src; // Source state
        QString dest; // Destination state
        QString condition; // Transittion condition
    };

    QHash<size_t,reprCondTr> allTransitionsConditions; ///< all conditions (transitions) identified by their ID

};
#endif // EDITORWINDOW_H
