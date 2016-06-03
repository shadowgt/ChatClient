#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(&m_login,SIGNAL(sendData(QString)),
            this,SLOT(reciveData(QString)));

    m_login.show();
}

MainWindow::~MainWindow()
{
    delete ui;
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
            else if(msgType == DEF_TYPE_CHANNEL_CHANGE)
            {
                ui->textEdit->clear();
                ui->textEdit->setFontUnderline(true);
                QByteArray arr;
                dataStream>>arr;
                QString str;
                str.append(arr.data()); // ID
                ui->textEdit->append(str);
                ui->textEdit->setFontUnderline(false);

                nextBlockSize = 0;
            }
            else if(msgType == DEF_TYPE_FILE)
            {
                qDebug() << "File received";
            }
            else if(msgType == DEF_TYPE_TRY_LOGIN)
            {

                quint16 value;
                dataStream>>value;
                if(value==1)
                {
                    QString str;
                    QTextStream textStream(&str);

                    textStream << "username \""
                               << CGBDataManager().Instance().getID()
                               << "\" "
                               << CGBDataManager().Instance().getIp()
                               << " "
                               << CGBDataManager().Instance().getPort()
                               << " port connected";

                    ui->textEdit->append(textStream.readAll());
                    ui->textEdit->append("#everyone channel joined");
                    this->show();
                    m_login.hide();
                }
                else
                {
                    //ui->textEdit->append("잘못된 연결");
                    qDebug() << " 잘못된 연결";

                    bool bCheck = false;

                    // if ID not null , you can try to login as geust
                    if(CGBDataManager::Instance().getID().size()!=0)
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Do you want to login as geust ?");
                        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                        int nCheck = msgBox.exec();
                        if(nCheck == QMessageBox::Ok)
                        {
                            connectServer(true);
                        }
                        else
                        {
                            bCheck = true;
                        }
                    }
                    else
                    {
                        bCheck = true;
                    }

                    if(bCheck = true)
                    {
                        this->hide();
                        m_login.show();
                        m_login.setEnabled(true);
                    }




                }
            }
            break;
        }
    }
    //qDebug() << "ok";
}

void MainWindow::reciveData(const QString &text)
{
    qDebug() << "test :" <<text;
    if(connectServer() == false)
    {
        m_login.setEnabled(true);
        m_login.show();
    }
}




void MainWindow::on_lineEdit_msg_returnPressed()
{

    QByteArray block;
    QDataStream dataStream(&block , QIODevice::ReadWrite);
    dataStream<<quint16(0);

    QString strData = ui->lineEdit_msg->text().toUtf8();
    QString strBuffer;
    if(strData.toUtf8().at(0) == '#')
    {
        dataStream<<DEF_TYPE_CHANNEL_CHANGE;
        strData.remove(QChar('#'), Qt::CaseInsensitive);

        foreach(QChar ch , strData)
        {
            if(ch == '#')
                continue;
            else if(ch == ' ')
                break;
            else
                strBuffer.append(ch);
        }
        strData = strBuffer;
        qDebug() << "채널" << strBuffer << "접속 시도";
    }
    else
    {
        dataStream<<DEF_TYPE_MESSAGE;
    }


    QByteArray buffer(strData.toUtf8());
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
    ui->lineEdit_msg->clear();
    int nCheck = m_socket.write(block);

    if(nCheck == -1)
    {
        m_login.show();
        m_login.setEnabled(true);
        this->hide();
    }
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
