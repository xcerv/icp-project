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
            loggingInstance->logMessage(msg, Qt::black);
            break;
        case QtWarningMsg:
            loggingInstance->logMessage(msg, Qt::darkYellow);
            break;
        case QtCriticalMsg:
            loggingInstance->logMessage(msg, Qt::red);
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

void LoggingWindow::logMessage(const QString &msg, Qt::GlobalColor color)
{
    QColor textColor = color;
    QString htmlMessage = QStringLiteral("<font color=\"%1\">%2</font>")
        .arg(textColor.name())
        .arg(Qt::convertFromPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + " - " + msg));

    this->ui->logDisplay->appendHtml(htmlMessage);
}

// By default there is no logging instance
LoggingWindow* LoggingWindow::loggingInstance = nullptr;