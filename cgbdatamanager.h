#ifndef CGBDATAMANAGER_H
#define CGBDATAMANAGER_H

#include<QtCore/QtGlobal>

/*
#if defined(CGBDATAMANAGER_H)
# define CGBDATAMANAGER_EXPORT Q_DECL_EXPORT
#else
# define CGBDATAMANAGER_EXPORT Q_DECL_IMPORT
#endif
*/


#include <QSharedData>
#include <QString>

class CGBDataManager
{

public:
    CGBDataManager();
    static CGBDataManager& Instance();
    void setID(QString i_str)
    {
        m_ID = i_str;
    }

    QString getID()
    {
        return m_ID;
    }
//private:
    QString m_ID;
};






#endif // CGBDATAMANAGER_H
