#include "clogin.h"
#include "cgbdatamanager.h"
#include "clogindlg.h"


CLogin::CLogin()
{

}

void CLogin::LoginIDCheck(QTcpSocket *s)
{
    CLoginDlg login;
    login.exec();
    QByteArray arr(CGBDataManager::Instance().getID().toUtf8());
    s->write(arr);
    s->flush();
}

void CLogin::tryConnect()
{

    QByteArray block;
    QDataStream dataStream(&block,QIODevice::ReadWrite);
    QByteArray arrID(CGBDataManager::Instance().getID().toUtf8());
    QByteArray arrPassword(CGBDataManager::Instance().getPassword().toUtf8());

    dataStream << quint16(0);
    dataStream << quint16(DEF_TYPE_TRY_LOGIN);
    dataStream << quint16(arrID.size()+arrPassword.size());
    dataStream << arrID;
    dataStream << arrPassword;
    dataStream.device()->seek(0);

    qDebug() << " block size : " << arrID.size() << " " << arrPassword.size();

    m_socket.write(block);
}


