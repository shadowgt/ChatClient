#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include "cgbdatamanager.h"
#include "clogin.h"
#include "define.h"
#include "csettingsdlg.h"
#include "clogindlg.h"

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
    void recvMsg();
    void reciveData(const QString &text);

private slots:

    void on_lineEdit_msg_returnPressed();
    void on_pushButton_fileSend_clicked();

private:
    CLoginDlg m_login;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
