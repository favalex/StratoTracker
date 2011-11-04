#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("StratoSpera");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("Develer");
    a.setOrganizationDomain("develer.com");

    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
