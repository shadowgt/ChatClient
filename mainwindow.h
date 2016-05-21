#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <cgbdatamanager.h>


#define DEF_TYPE_MESSAGE quint16(1)
#define DEF_TYPE_FILE quint16(2)



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

    void on_pushButton_fileSend_clicked();

private:
    QTcpSocket m_socket;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
