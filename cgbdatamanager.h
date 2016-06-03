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

    QString getID()
    {
        return m_ID;
    }

    QString getPassword()
    {
        return m_Password;
    }

    QString getIp()
    {
        return m_Ip;
    }

    QString getPort()
    {
        return m_Port;
    }

private:
    QString m_ID , m_Password , m_Port , m_Ip;



};






#endif // CGBDATAMANAGER_H
