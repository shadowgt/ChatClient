#ifndef CSETTINGSDLG_H
#define CSETTINGSDLG_H

#include <QDialog>
#include "define.h"
#include <QAbstractButton>

namespace Ui {
class CSettingsDlg;
}

class CSettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CSettingsDlg(QWidget *parent = 0);
    ~CSettingsDlg();\

    void setDataMng(); // CGBDataManager 에 ip , port 저장 한다.

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::CSettingsDlg *ui;
};

#endif // CSettingsDlg_H
