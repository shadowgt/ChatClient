#include "clogin.h"
#include "cgbdatamanager.h"
#include "clogindlg.h"


CLogin::CLogin()
{

}

void CLogin::sendLoginData(bool i_b)
{

    QByteArray block;
    QDataStream dataStream(&block,QIODevice::ReadWrite);
    QByteArray arrID(CGBDataManager::Instance().getID().toUtf8());
    QByteArray arrPassword;


    if(i_b == true)
    {
        QString str = CGBDataManager::Instance().getID().toUtf8();
        str.append("_guest");
        CGBDataManager::Instance().setID(str);
        arrPassword.append("guest");
    }
    else
    {
        arrPassword.append(CGBDataManager::Instance().getPassword().toUtf8());
    }


    dataStream << quint16(0);
    dataStream << quint16(DEF_TYPE_TRY_LOGIN);

    dataStream << quint16(arrID.size()+arrPassword.size());

    dataStream << arrID;
    dataStream << arrPassword;
    dataStream.device()->seek(0);

    qDebug() << "is open "<<m_socket.isOpen();

    qDebug() << " block size : " << arrID.size() << " " << arrPassword.size();


    qint64 flag = m_socket.write(block);
    if(flag == -1)
    {
        qDebug() << "로그인 블록 전송 실패";
    }
    else
        qDebug() << "로그인 블록 전송";
}

bool CLogin::connectServer(bool i_b)
{
    bool bCheck = false;
    QString host_str(CGBDataManager::Instance().getIp());
    int port = CGBDataManager::Instance().getPort().toInt();
    m_socket.connectToHost(host_str,port);


    if(m_socket.waitForConnected(1000))
    {
        bCheck = true;
        qDebug() << "로그인시도";
        sendLoginData(i_b);

    }
    else
    {
        qDebug() << "연결 안됨";
    }



    return bCheck;
}


