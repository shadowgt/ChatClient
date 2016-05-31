#include "clogindlg.h"
#include "ui_logindlg.h"
#include "cgbdatamanager.h"
#include <QDebug>
CLoginDlg::CLoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDlg)
{
    ui->setupUi(this);
}

CLoginDlg::~CLoginDlg()
{
    delete ui;
}

void CLoginDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    CGBDataManager::Instance().setID(ui->lineEdit_ID->text());
    CGBDataManager::Instance().setPassword(ui->lineEdit_Password->text());
}
