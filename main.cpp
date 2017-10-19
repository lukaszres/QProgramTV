#include "mainwindow.h"
#include <QApplication>
#include "versionchecker.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    float version{1.3};
    QString versionURL{"https://raw.githubusercontent.com/lukaszres/QProgramTV/master/version.txt"};

    QApplication a(argc, argv);

    VersionChecker vChecker;
    vChecker.setCurrentVersion(version);
    vChecker.setFileURL(versionURL);
    if (vChecker.isActual())
        qDebug()<<"Twoja wersja jest aktualna.";
    else
        qDebug()<<"Twoja wersja jest nieaktualna";

    MainWindow w;
    w.show();

    return a.exec();
}
