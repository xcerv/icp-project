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
#include "loggingwindow.h"

#include <QTextStream>
#include <QDateTime>
#include <QObject>
#include <QtGlobal>

LoggingWindow::LoggingWindow(QWidget *parent)
    :
    QWidget(parent),
    originalHandler(qInstallMessageHandler(0)),
    ui(new Ui::LoggingWindow)
{
    ui->setupUi(this);

    // Connect Clear button
    connect(ui->logClear, &QPushButton::clicked,this, &LoggingWindow::clearLog);

    // Start logging
    this->startLogging();
}


LoggingWindow::~LoggingWindow()
{
    delete ui;
}

void LoggingWindow::loggingMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type) {
        case QtInfoMsg:
            loggingInstance->ui->logDisplay->setPlainText(msg);
            break;
        default:
            loggingInstance->originalHandler(type, context, msg);
            break;
    }
}

void LoggingWindow::startLogging()
{
    if(loggingInstance != nullptr) return; 
    loggingInstance = this;
    qInstallMessageHandler(loggingMessageHandler);
}

void LoggingWindow::stopLogging()
{
    loggingInstance = nullptr;
    qInstallMessageHandler(0);
}

void LoggingWindow::clearLog()
{
    ui->logDisplay->clear();
}

// By default there is no logging instance
LoggingWindow* LoggingWindow::loggingInstance = nullptr;