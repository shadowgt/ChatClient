#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

private:
    QTcpSocket socket;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
