/**
 * Project name: ICP Project 2024/2025
 *
 * @file loggingwindow.cpp
 * @author  xcervia00
 *
 * @brief Window for logging events
 *
 */

#include "view/logging_window/loggingwindow.h"
#include "ui_loggingwindow.h"

LoggingWindow::LoggingWindow(QWidget *parent)
    :
    QWidget(parent),
    ui(new Ui::LoggingWindow)
{
    ui->setupUi(this);
}

LoggingWindow::~LoggingWindow()
{
    delete ui;
}
