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
    QByteArray arrPassword;
    QByteArray arrID(m_ID.toUtf8());
    if(i_b == true)
    {
        QString str = m_ID.toUtf8();
        str.append("_guest");
        m_ID = str;
        arrPassword.append("guest");
    }
    else
    {
        arrPassword.append(m_Password.toUtf8());
    }




    dataStream << quint16(0);
    dataStream << quint16(DEF_TYPE_TRY_LOGIN);

    dataStream << quint16(arrID.size()+arrPassword.size());

    dataStream << arrID;

    // Password Sha3 알고리즘으로 암호화
    QCryptographicHash * hash;
    QString strPassword(arrPassword.data());
    QByteArray arrHash(hash->hash(strPassword.toLatin1(), QCryptographicHash::Sha3_512));
    dataStream << arrHash.toHex();

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

void CLogin::sendSignUp()
{
    QByteArray block;
    QDataStream dataStream(&block,QIODevice::ReadWrite);
    QByteArray arrID(m_ID.toUtf8());
    QByteArray arrPassword(m_Password.toUtf8());
    QByteArray arrName(m_Name.toUtf8());
    quint16 usStatus = quint16(m_Status);

    dataStream << quint16(0);
    dataStream << quint16(DEF_TYPE_SIGN_UP);

    dataStream << quint16(arrID.size()+arrPassword.size());

    dataStream << arrID;

    // Password Sha3 알고리즘으로 암호화
    QCryptographicHash * hash;
    QString strPassword(arrPassword.data());
    QByteArray arrHash(hash->hash(strPassword.toLatin1(), QCryptographicHash::Sha3_512));
    dataStream << arrHash.toHex();

    dataStream << arrName;
    dataStream << usStatus;
    dataStream.device()->seek(0);

    qDebug() << " block size : " << arrID.size() << " " << arrPassword.size();


    qint64 flag = m_socket.write(block);
    if(flag == -1)
    {
        qDebug() << "회원가입 블록 전송 실패";
    }
    else
        qDebug() << "회원가입 블록 전송";
}

bool CLogin::connectServer(bool i_b)
{
    bool bCheck = false;
    QString host_str(m_Ip);
    int port = m_Port.toInt();
    m_socket.connectToHost(host_str,port);


    if(m_socket.waitForConnected(1000))
    {
        bCheck = true;
        qDebug() << "로그인시도";


    }
    else
    {
        qDebug() << "연결 안됨";
    }



    return bCheck;
}


