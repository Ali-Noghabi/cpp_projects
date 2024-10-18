#include "mainwindow.h"

#include <QApplication>

int  main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Datall");
    QCoreApplication::setApplicationName("DataGenerator");

    QApplication  a(argc, argv);
    MainWindow    w;

    w.showMaximized();

    return a.exec();
}
