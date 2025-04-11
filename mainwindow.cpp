#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mytextoutput1 = ui->textEdit;
    mytextoutput2 = ui->textEdit_2;
    mytextoutputforthreads = ui->textEdit_3;
    mytextoutputforthreads2 = ui->textEdit_4;

    mytextoutput1->setReadOnly(true);
    mytextoutput2->setReadOnly(true);
    mytextoutputforthreads->setReadOnly(true);

    myprocess = new QProcess(this);

    connect(myprocess, &QProcess::readyReadStandardOutput, [=]()
            { mytextoutput1->append(myprocess->readAllStandardOutput()); });

    connect(myprocess, &QProcess::errorOccurred, [=]()
            { mytextoutput2->append(myprocess->errorString()); });

    connect(myprocess, &QProcess::finished, [=](int exitcode, QProcess::ExitStatus exitstatus)
            {
                mytextoutput2->append("Process finished with exit code: " + QString::number(exitcode));
                mytextoutput2->append("Exit status: " + QString(exitstatus == QProcess::NormalExit ? "Normal Exit" : "Crash Exit")); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    try
    {
        myprocess->start("ping", QStringList() << "google.com");
        // myprocess->start("/usr/bin/mousepad");
        mytextoutput2->append("Process started");
    }
    catch (const std::exception &e)
    {
        // qDebug() << "error ";
        mytextoutput2->append("error " + QString(e.what()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (myprocess->state() == QProcess::Running)
    {
        // qDebug() << "the process is working";
        mytextoutput2->append("the process is working");
    }
    else
    {
        // qDebug() << "process not running";
        mytextoutput2->append("process not running");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (myprocess->state() == QProcess::Running)
    {
        myprocess->terminate();
        if (!myprocess->waitForFinished(3000))
        {                      // Wait for up to 3 seconds
            myprocess->kill(); // Forcefully kill the process if it doesn't terminate
        }
        // qDebug() << "Process terminated.";
        mytextoutput2->append("Process terminated.");
    }

    stopThreadExecution();
}


void MainWindow::myfunc()
{
    stopThread = false;
    currentFuture = QtConcurrent::run([this]() {
        for (int i = 0; i < 10 && !stopThread; i++) {
            QMetaObject::invokeMethod(mytextoutputforthreads, [this,i]() {
                mytextoutputforthreads->append(QString::number(i));
            }, Qt::QueuedConnection);
            QThread::msleep(1000);
        }
        
        // Optional completion notification
        QMetaObject::invokeMethod(this, [this]() {
            if (stopThread) {
                mytextoutputforthreads->append("Thread stopped");
            } else {
                mytextoutputforthreads->append("Task completed");
            }
        }, Qt::QueuedConnection);
    });
}

void MainWindow::myfunc2()
{
    stopThread = false;
    currentFuture = QtConcurrent::run([this]() {
        for (int i = 0; i < 10 && !stopThread; i++) {
            QMetaObject::invokeMethod(mytextoutputforthreads2, [this,i]() {
                mytextoutputforthreads2->append(QString::number(i));
            }, Qt::QueuedConnection);
            QThread::msleep(1000);
        }
        
        // Optional completion notification
        QMetaObject::invokeMethod(this, [this]() {
            if (stopThread) {
                mytextoutputforthreads2->append("Thread stopped");
            } else {
                mytextoutputforthreads2->append("Task completed");
            }
        }, Qt::QueuedConnection);
    });
}

void MainWindow::stopThreadExecution()
{
    stopThread = true;
    if (currentFuture.isRunning()) {
        currentFuture.waitForFinished();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    myfunc(); // This won't block the GUI
}

void MainWindow::on_pushButton_5_clicked()
{
    myfunc2();
}

