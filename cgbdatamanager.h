#ifndef CGBDATAMANAGER_H
#define CGBDATAMANAGER_H

#include<QtCore/QtGlobal>
#include<QTcpSocket>
#include <QSharedData>
#include <QString>
#include "define.h"
class CGBDataManager
{

public:
    CGBDataManager();
    static CGBDataManager& Instance();
    QTcpSocket * m_Socket;
    void setID(QString i_str)
    {
        m_ID = i_str;
    }
    void setPassword(QString i_str)
    {
        m_Password = i_str;
    }

    void setIp(QString i_str)
    {
        m_Ip = i_str;
    }
    void setPort(QString i_str)
    {
        m_Port = i_str;
    }

    void setName(QString i_str)
    {
        m_Name = i_str;
    }

    void setStatus(int i_n)
    {
        m_Status = i_n;
    }



    QString getID()
    {
        QString buf = m_ID;
        m_ID.clear();

        return buf;
    }

    QString getPassword()
    {
        QString buf = m_Password;
        m_Password.clear();

        return buf;
    }

    QString getIp()
    {
        QString buf = m_Ip;
        m_Ip.clear();

        return buf;
    }

    QString getPort()
    {
        QString buf = m_Port;
        m_Port.clear();

        return buf;
    }

    QString getName()
    {
        QString buf = m_Name;
        m_Name.clear();

        return buf;
    }

    int getStatus()
    {
        int buf = m_Status;
        m_Status = 0;

        return buf;
    }

private:
    QString m_ID , m_Password , m_Port , m_Ip, m_Name;
    int m_Status;



};






#endif // CGBDATAMANAGER_H
