#ifndef CLOGINDLG_H
#define CLOGINDLG_H

#include <QDialog>
#include <cgbdatamanager.h>
#include <QAbstractButton>
#include "csettingsdlg.h"
#include "csignupdlg.h"
#include "define.h"

namespace Ui {
class CLoginDlg;
}

class CLoginDlg : public QDialog
{
    Q_OBJECT

signals:
    void sendData(const QString &text);

public:
    explicit CLoginDlg(QWidget *parent = 0);
    ~CLoginDlg();

    CSettingsDlg m_SettingsDlg;
    CSignUpDlg m_SignUpDlg;

    void initData();


private slots:
    void on_pushButton_settings_clicked();
    void on_pushButton_OK_clicked();

    void on_pushButton_SignUp_clicked();

private:
    Ui::CLoginDlg *ui;

};

#endif // CLOGINDLG_H
