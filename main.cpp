#include "mainwindow.h"
#include <QApplication>
#include "clogindlg.h"

//QSharedDataPointer<CGBDataManager> d;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    return a.exec();
}
