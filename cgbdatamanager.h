#ifndef CGBDATAMANAGER_H
#define CGBDATAMANAGER_H

#include<QtCore/QtGlobal>

#include <QSharedData>
#include <QString>
#include "define.h"
class CGBDataManager
{

public:
    CGBDataManager();
    static CGBDataManager& Instance();
    void setID(QString i_str)
    {
        m_ID = i_str;
    }
    void setPassword(QString i_str)
    {
        m_Password = i_str;
    }

    QString getID()
    {
        return m_ID;
    }

    QString getPassword()
    {
        return m_Password;
    }
private:
    QString m_ID;
    QString m_Password;
};






#endif // CGBDATAMANAGER_H
