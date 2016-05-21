#ifndef CLOGINDLG_H
#define CLOGINDLG_H

#include <QDialog>
#include <cgbdatamanager.h>
#include <QAbstractButton>

namespace Ui {
class CLoginDlg;
}

class CLoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CLoginDlg(QWidget *parent = 0);
    ~CLoginDlg();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::CLoginDlg *ui;
};

#endif // CLOGINDLG_H
