#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));


    //setting
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_port->setText("30000");
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
    QTcpSocket * tcpSocket = (QTcpSocket*)sender();
    QByteArray arr(tcpSocket->readAll());
    stMsg * msg = (stMsg*)arr.data();
    QString str(msg->msg);
    ui->textEdit->append(msg->msg);
}


void MainWindow::on_pushButton_connect_clicked()
{
    QString host_str(ui->lineEdit_ip->text());
    QHostAddress host(host_str);
    int port = ui->lineEdit_port->text().toInt();
    m_socket.connectToHost(host,port);


    ui->textEdit->append(QString("연결되었습니다."));
}

void MainWindow::on_lineEdit_msg_returnPressed()
{
    QByteArray block;
    QDataStream dataStream(&block , QIODevice::ReadWrite);

    dataStream<<quint16(0);
    dataStream << ui->lineEdit_msg->text();
    dataStream.device()->seek(0);
    dataStream<<quint16(block.size() - sizeof(quint16));

    qDebug() << "설정된 블록 사이즈" << quint16(block.size() - sizeof(quint16));
    qDebug() << "블록 사이즈 :" << quint16(block.size());

    //소켓으로 전송
    m_socket.write(block);
    ui->lineEdit_msg->clear();

    return;
}
