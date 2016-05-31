#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include "cgbdatamanager.h"
#include "clogin.h"
#include "define.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow ,CLogin
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void sendMessage();
    void recvMsg();

private slots:
    void on_pushButton_connect_clicked();
    void on_lineEdit_msg_returnPressed();
    void on_pushButton_fileSend_clicked();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
