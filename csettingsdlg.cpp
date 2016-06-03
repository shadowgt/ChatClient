#include "csettingsdlg.h"
#include "ui_csettingsdlg.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>


CSettingsDlg::CSettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSettingsDlg)
{
    ui->setupUi(this);

    QFile file(DEF_FILE_PATH);
    QFileInfo check_file(DEF_FILE_PATH);
    bool bNewFile = false;

    if(check_file.exists() && check_file.isFile())
    {
        if(check_file.size() == 0)
        {
            bNewFile = true;
            qDebug() << "new settings file";
        }
    }
    else
    {
         bNewFile = true;
         qDebug() << "new settings file";
    }

    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in(&file);
        if(bNewFile)
        {
            in<< "IP ADDRESSE";
            in<< "127.0.0.1";
            in<< "PORT NUMBER";
            in<< "30000";
            ui->lineEdit_ip->setText("127.0.0.1");
            ui->lineEdit_port->setText("30000");
        }
        else
        {
            int count = 0;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if(count == 1)
                {
                    ui->lineEdit_ip->setText(line);
                }
                else if(count == 3)
                {
                    ui->lineEdit_port->setText(line);
                }

                count++;
            }
        }
    }
    else
    {
        qDebug("it was not open");
    }
    file.close();
}

CSettingsDlg::~CSettingsDlg()
{
    delete ui;
}

void CSettingsDlg::setDataMng()
{
    CGBDataManager::Instance().setIp(ui->lineEdit_ip->text());
    CGBDataManager::Instance().setPort(ui->lineEdit_port->text());
    qDebug() << "set IP "<< ui->lineEdit_ip->text();
    qDebug() << "set port" <<  ui->lineEdit_port->text();
}


void CSettingsDlg::on_pushButton_ok_clicked()
{
    QFile file(DEF_FILE_PATH);

    if(file.open(QIODevice::ReadWrite  | QIODevice::Text))
    {
        QTextStream in(&file);
        in<< "IP ADDRESSE \n";
        in<< ui->lineEdit_ip->text().toUtf8();
        in<< "\n";
        in<< "PORT NUMBER \n";
        in<< ui->lineEdit_port->text().toUtf8();

    }
    else
    {
        qDebug("it was not open");
    }

    CGBDataManager::Instance().setIp(ui->lineEdit_ip->text());
    CGBDataManager::Instance().setPort(ui->lineEdit_port->text());

    file.close();
    close();
}

void CSettingsDlg::on_pushButton_cancel_clicked()
{
    close();
}
