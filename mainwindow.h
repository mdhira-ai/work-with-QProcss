#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked(); //thread 1

    void on_pushButton_5_clicked(); //thread 2

signals:


private:
    Ui::MainWindow *ui;
    QProcess *myprocess;
    QTextEdit *mytextoutput1;
    QTextEdit *mytextoutput2;
    QTextEdit *mytextoutputforthreads;
    QTextEdit *mytextoutputforthreads2;
    void myfunc();
    void myfunc2();

    // for stop thread
    void stopThreadExecution();
    QFuture<void> currentFuture;
    std::atomic<bool> stopThread = false;
};
#endif // MAINWINDOW_H
