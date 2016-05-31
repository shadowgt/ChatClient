#ifndef CLOGIN_H
#define CLOGIN_H
#include<QTcpSocket>
#include "define.h"

class CLogin
{
public:
    CLogin();

    void LoginIDCheck(QTcpSocket * s);
    void tryConnect();

protected:
    QTcpSocket m_socket;


};

#endif // CLOGIN_H
