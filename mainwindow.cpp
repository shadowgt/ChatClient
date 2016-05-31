#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "clogindlg.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));

    LoginIDCheck(&m_socket);

    this->show();

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
    QDataStream dataStream(tcpSocket);


    quint16 nextBlockSize = 0;
    int msgType = 0;

    while(true)
    {
        if(nextBlockSize == 0)
        {
            //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                ;
            else
            {
                dataStream>>msgType;
                dataStream>>nextBlockSize;

                qDebug() << " received file size of block :" << nextBlockSize;
            }
            continue;
        }
        //nextBlcokSize가 도착하면 사이즈만큼 데이터가 도착했는지 확인
        else if(tcpSocket->bytesAvailable() < nextBlockSize)
            continue;

        //데이터를 표시
        else if(tcpSocket->bytesAvailable() >= nextBlockSize)
        {
            if(msgType == DEF_TYPE_MESSAGE)
            {
                QByteArray arr;
                dataStream>>arr;
                QString str;
                str.append(arr.data()); // ID
                ui->textEdit->append(str);
                nextBlockSize = 0;

            }
            else if(msgType == DEF_TYPE_FILE)
            {
                qDebug() << "File received";
            }
            else if(msgType == DEF_TYPE_TRY_LOGIN)
            {
                QByteArray arr;
                quint16 value;
                dataStream>>value;
                if(value==1)
                {
                    ui->textEdit->append("연결되었습니다.");
                }
                else
                {
                    ui->textEdit->append("잘못된 연결");
                    //tryConnect();
                }
            }
            break;
        }
    }
    //qDebug() << "ok";
}


void MainWindow::on_pushButton_connect_clicked()
{
    QString host_str(ui->lineEdit_ip->text());
    int port = ui->lineEdit_port->text().toInt();
    m_socket.connectToHost(host_str,port);


    if(m_socket.waitForConnected(1000))
    {
        tryConnect();
    }
    else
    {
        qDebug() << "연결 안됨";
    }
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
/*
    quint16 test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> test;
    qDebug() << test;
    dataStream >> chk;
    qDebug() << chk.data();
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
    QDataStream dataStream(&block , QIODevice::ReadOnly);
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
