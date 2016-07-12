#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(&(m_login.m_SignUpDlg),SIGNAL(sendData(QString)),
            this,SLOT(reciveData(QString)));
    connect(&(m_login.m_SettingsDlg),SIGNAL(sendData(QString)),
            this,SLOT(reciveData(QString)));
    connect(&m_login,SIGNAL(sendData(QString)),
            this,SLOT(reciveData(QString)));

    m_login.show();
    emit m_login.sendData("Set_Settings");
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
            qDebug() << "nextBlockSize == 0";
            //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                qDebug() << "tcpSocket->bytesAvailable() < sizeof(quint16)";
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
                QString strID = arr.data();
                dataStream>>arr;
                QString strMessage = arr.data();

                QString buffer;
                buffer.append(strID);
                buffer.append(" : ");
                buffer.append(strMessage);


                //str.append(arr.toHex()); // ID
                ui->textEdit->append(buffer);

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

                requireChatMember();

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
                               << m_ID
                               << "\" "
                               << m_Ip
                               << " "
                               << m_Port
                               << " port connected";

                    ui->textEdit->append(textStream.readAll());
                    ui->textEdit->append("#everyone channel joined");
                    this->show();
                    m_login.hide();
                    m_login.clear();

                    requireChatMember();
                }
                else
                {
                    //ui->textEdit->append("잘못된 연결");
                    qDebug() << " 잘못된 연결";

                    bool bCheck = false;

                    // if ID not null , you can try to login as geust
                    if(m_ID!=0&&m_Password==0)
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Do you want to login as geust ?");
                        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                        int nCheck = msgBox.exec();
                        if(nCheck == QMessageBox::Ok)
                        {
                            connectServer();
                            sendLoginData(true);
                        }
                        else
                        {

                            bCheck = true;
                        }
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setText("등록되지 않은 ID 이거나 잘못된 Password 를 입력 하셨습니다.");
                        msgBox.exec();
                        bCheck = true;
                    }

                    if(bCheck == true)
                    {
                        this->hide();
                        m_login.show();
                        m_login.setEnabled(true);
                    }
                }
            }
            else if(msgType == DEF_TYPE_SIGN_UP)
            {
                QByteArray arr;
                dataStream>>arr;
                QString str;
                str.append(arr.data()); // ID

                if(str.compare("DUPLICATE_ID")==0)
                {
                    QMessageBox msgBox;
                    msgBox.setText("사용할수 없는 ID 입니다.");
                    msgBox.exec();
                }
                else if(str.compare("DUPLICATE_NAME")==0)
                {
                    QMessageBox msgBox;
                    msgBox.setText("사용할수 없는 Name 입니다.");
                    msgBox.exec();
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText(m_ID+"님 회원가입을 축하드립니다.");
                    msgBox.exec();
                    m_login.m_SignUpDlg.initData();
                    m_login.m_SignUpDlg.close();
                }

                qDebug() << str;
            }
            else if(msgType == DEF_TYPE_REQUIRE_MEMBERS_INFO)
            {
                QByteArray arr;
                quint16 nSize;
                dataStream>>nSize;

                qDebug() << "size :" << nSize;
                QString str;

                ui->listWidget->clear();
                for(int i = 0; i < nSize ;i++)
                {
                    dataStream>>arr;
                    str = arr.data();
                    ui->listWidget->addItem(str);
                }
            }
            nextBlockSize = 0;
            break;
        }
    }
    //qDebug() << "ok";
}

void MainWindow::reciveData(const QString &text)
{
    qDebug() << "test :" <<text;

    if(text.compare("Login_Dialog_Button_OK") == 0)
    {
        m_ID = CGBDataManager::Instance().getID();
        m_Password = CGBDataManager::Instance().getPassword();


        qDebug() <<  m_Port << " " << m_Ip;

        if(connectServer() == false)
        {
            m_login.setEnabled(true);
            m_login.show();
        }
        else
        {
            sendLoginData();
        }
    }
    else if(text.compare("SignUp_Dialog_Button_Accept") == 0)
    {
        m_ID = CGBDataManager::Instance().getID();
        m_Password = CGBDataManager::Instance().getPassword();
        m_Name = CGBDataManager::Instance().getName();
        m_Status = CGBDataManager::Instance().getStatus();

        if(connectServer() == false)
        {

        }
        else
        {
            sendSignUp();
        }


    }
    else if(text.compare("Set_Settings")== 0)
    {
        m_Port = CGBDataManager::Instance().getPort();
        m_Ip = CGBDataManager::Instance().getIp();
        qDebug() <<  m_Port << " " << m_Ip;
    }
}

void MainWindow::requireChatMember()
{
    QString str("requireChatMember");
    QByteArray block,buffer;
    QDataStream dataStream(&block , QIODevice::ReadWrite);
    dataStream<<quint16(0);

    dataStream<<DEF_TYPE_REQUIRE_MEMBERS_INFO;
    buffer.append(str);
    dataStream<<quint16(buffer.size());
    dataStream<<str.toUtf8();

    qDebug() << "requireChatMember";
    m_socket.write(block);
}


void MainWindow::on_lineEdit_msg_returnPressed()
{

    QByteArray block;
    QDataStream dataStream(&block , QIODevice::ReadWrite);
    dataStream<<quint16(0);

    QString strData = ui->lineEdit_msg->text().toUtf8();


    if(strData.size()==0)
        return;

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

        QByteArray buffer(strData.toUtf8());
        dataStream<<quint16(buffer.size()+m_ID.toUtf8().size());
        qDebug() << m_ID.toUtf8();
        dataStream<<m_ID.toUtf8();
        dataStream<<buffer;
    }
    else
    {
        dataStream<<DEF_TYPE_MESSAGE;
        strData.toUtf8();
        dataStream<<quint16(strData.toUtf8().size()+m_ID.toUtf8().size());
        qDebug() << m_ID.toUtf8();
        dataStream<<m_ID.toUtf8();
        dataStream<<strData.toUtf8();
    }




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

void MainWindow::on_pushButton_msgSend_clicked()
{
    on_lineEdit_msg_returnPressed();
}
