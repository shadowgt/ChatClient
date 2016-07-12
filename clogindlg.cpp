#include "clogindlg.h"
#include "ui_logindlg.h"
#include "cgbdatamanager.h"
#include <QDebug>
CLoginDlg::CLoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDlg)
{
    ui->setupUi(this);

    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    m_SettingsDlg.setDataMng();
}

CLoginDlg::~CLoginDlg()
{
    delete ui;
}

void CLoginDlg::clear()
{
    ui->lineEdit_ID->setText("");
    ui->lineEdit_Password->setText("");
}


void CLoginDlg::on_pushButton_settings_clicked()
{
    m_SettingsDlg.exec();
}

void CLoginDlg::on_pushButton_OK_clicked()
{

    CGBDataManager::Instance().setID(ui->lineEdit_ID->text());
    CGBDataManager::Instance().setPassword(ui->lineEdit_Password->text());

    if(ui->lineEdit_ID->text().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("ID 를 입력해주세요.");
        msgBox.exec();
    }
    else
    {
        this->setEnabled(false);
        emit this->sendData("Login_Dialog_Button_OK");
    }
}


void CLoginDlg::on_pushButton_SignUp_clicked()
{
    m_SignUpDlg.exec();
}
