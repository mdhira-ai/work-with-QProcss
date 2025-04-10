#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mytextoutput1 = ui->textEdit;
    mytextoutput2 = ui->textEdit_2;

    mytextoutput1->setReadOnly(true);
    mytextoutput2->setReadOnly(true);
    myprocess = new QProcess(this);

    connect(myprocess, &QProcess::readyReadStandardOutput, [=]()
            { mytextoutput1->append(myprocess->readAllStandardOutput()); });

    connect(myprocess, &QProcess::errorOccurred, [=]()
            { mytextoutput2->append("process error occured"); });


    connect(myprocess, &QProcess::finished, [=](int exitcode, QProcess::ExitStatus exitstatus)
            {
                mytextoutput2->append("Process finished with exit code: " + QString::number(exitcode));
                mytextoutput2->append("Exit status: " + QString(exitstatus == QProcess::NormalExit ? "Normal Exit" : "Crash Exit"));
            });
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
}
