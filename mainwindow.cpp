#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{

}


void MainWindow::recvMsg()
{
    QTcpSocket * s = (QTcpSocket*)sender();
    QByteArray arr(s->readAll());
    QString str(arr);
    ui->textEdit->append(str);

}


void MainWindow::on_pushButton_connect_clicked()
{
    QString host_str(ui->lineEdit_ip->text());
    QHostAddress host(host_str);
    int port = ui->lineEdit_port->text().toInt();
    socket.connectToHost(host,port);
    ui->textEdit->append(QString("연결되었습니다."));
}

void MainWindow::on_lineEdit_msg_returnPressed()
{
    QByteArray arr(ui->lineEdit_msg->text().toUtf8());
    socket.write(arr);
    socket.flush();
    ui->lineEdit_msg->clear();
}
