#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));

    //setting
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_port->setText("30000");



    //ui->textEdit->setText();
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
    QByteArray buffer(ui->lineEdit_msg->text().toUtf8());

    QByteArray block ,chk;
    QDataStream dataStream(&block , QIODevice::ReadWrite);
    dataStream<<quint16(0);
    dataStream<<DEF_TYPE_MESSAGE;
    dataStream<<quint16(buffer.size());
    qDebug() << CGBDataManager::Instance().getID().toUtf8();
    dataStream<<CGBDataManager::Instance().getID().toUtf8();
    dataStream<<buffer;
    dataStream.device()->seek(0);

    quint16 test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> chk;
    qDebug() << chk.data();

    /*  sequence of dataStream ,if send Message on dataStream
    *
    * 0 >> DEF_TYPE_MESSAGE >> ID >> BlockSize >> Block
    */
    //소켓으로 전송
    m_socket.write(block);
    ui->lineEdit_msg->clear();
}

void MainWindow::on_pushButton_fileSend_clicked()
{
    QStringList fileName;


    QFileDialog dialog(this);
    dialog.setDirectory("c:/Users/Bs/Documents/Search_Word/img/");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::DontUseNativeDialog); // NativeDialog has warning

    if (dialog.exec())
        fileName = dialog.selectedFiles();

    QByteArray block;
    QDataStream dataStream(&block , QIODevice::ReadWrite);
    dataStream<<quint16(0);


    QStringListIterator javaStyleIterator(fileName);
    while (javaStyleIterator.hasNext())
    {
        //qDebug() << javaStyleIterator.next().toLocal8Bit().constData() << endl;
        QFile file(javaStyleIterator.next().toLocal8Bit().constData());
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "This File can't open";
            continue;
        }

        QByteArray arr = file.readAll();
        block.append(arr);
    }

    dataStream.device()->seek(0);
    dataStream<<quint16(block.size() - sizeof(quint16));

    qDebug() << "설정된 블록 사이즈" << quint16(block.size() - sizeof(quint16));
    qDebug() << "블록 사이즈 :" << quint16(block.size());

    //소켓으로 전송
    m_socket.write(block);
    ui->lineEdit_msg->clear();
}
