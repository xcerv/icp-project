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
/**
 * @brief The logger window
 */
class LoggingWindow : public QWidget
{
    Q_OBJECT

public:
    static LoggingWindow * loggingInstance; ///< What is the current logger object
    QtMessageHandler originalHandler; ///< The default handler of messages

    explicit LoggingWindow(QWidget *parent = nullptr);
    ~LoggingWindow();

    /**
     * @brief Handler for all qMessages ==> redirects them to logger 
     * @param type The type of the message
     * @param context Context of the message
     * @param msg The message itself
     */
    static void loggingMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    /**
     * @brief Starts logging
     */
    void startLogging();
    /**
     * @brief Stops logging
     */
    void stopLogging();
    /**
     * @brief Clears up the entire log
     */
    void clearLog();

    /**
     * @brief Prints message to the logging window
     * @param msg The log message
     * @param color The color of the message
     */
    void logMessage(const QString &msg, Qt::GlobalColor color);


private:
    Ui::LoggingWindow *ui; ///< The UI element
};

#endif // LOGGINGWINDOW_H
