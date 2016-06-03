#include "clogindlg.h"
#include "ui_logindlg.h"
#include "cgbdatamanager.h"
#include <QDebug>
CLoginDlg::CLoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDlg)
{
    ui->setupUi(this);

    CSettingsDlg SettingsDlg;
    SettingsDlg.setDataMng(); //get port , ip
    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);

}

CLoginDlg::~CLoginDlg()
{
    delete ui;
}


void CLoginDlg::on_pushButton_settings_clicked()
{
    CSettingsDlg SettingsDlg;
    SettingsDlg.exec();
}

void CLoginDlg::on_pushButton_OK_clicked()
{
    CGBDataManager::Instance().setID(ui->lineEdit_ID->text());
    CGBDataManager::Instance().setPassword(ui->lineEdit_Password->text());
    this->setEnabled(false);
    emit this->sendData("Start");
}

void CLoginDlg::on_pushButton_exit_clicked()
{
    this->close();
}
