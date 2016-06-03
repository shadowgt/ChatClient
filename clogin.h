#ifndef CLOGIN_H
#define CLOGIN_H
#include<QTcpSocket>
#include "define.h"

class CLogin
{
public:
    CLogin();

    void sendLoginData(bool i_b = false);
    bool connectServer(bool i_b = false);

protected:
    QTcpSocket m_socket;


};

#endif // CLOGIN_H
