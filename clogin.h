#ifndef CLOGIN_H
#define CLOGIN_H
#include<QTcpSocket>
#include "define.h"

class CLogin
{
public:
    CLogin();

    void sendLoginData(bool i_b = false);
    void sendSignUp();

    bool connectServer(bool i_b = false);

protected:
    QTcpSocket m_socket;
    QString m_ID , m_Password , m_Port , m_Ip, m_Name;
    int m_Status;
};

#endif // CLOGIN_H
