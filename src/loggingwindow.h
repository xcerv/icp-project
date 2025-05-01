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

namespace Ui {
class LoggingWindow;
}

class LoggingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoggingWindow(QWidget *parent = nullptr);
    ~LoggingWindow();

private:
    Ui::LoggingWindow *ui;
};

#endif // LOGGINGWINDOW_H
