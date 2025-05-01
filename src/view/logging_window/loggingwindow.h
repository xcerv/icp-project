/**
 * Project name: ICP Project 2024/2025
 *
 * @file loggingwindow.h
 * @author  xcervia00
 *
 * @brief Window for logging events (header)
 *
 */

#ifndef LOGGINGWINDOW_H
#define LOGGINGWINDOW_H

#include <QWidget>
#include <QMessageLogContext>

namespace Ui {
class LoggingWindow;
}

class LoggingWindow : public QWidget
{
    Q_OBJECT

public:
    static LoggingWindow * loggingInstance;
    QtMessageHandler originalHandler;

    explicit LoggingWindow(QWidget *parent = nullptr);
    ~LoggingWindow();

    /**
     * @brief Handler for all 
     * @param type 
     * @param context 
     * @param msg 
     */
    static void loggingMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    void startLogging();
    void stopLogging();
    void clearLog();


private:
    Ui::LoggingWindow *ui;
};

#endif // LOGGINGWINDOW_H
