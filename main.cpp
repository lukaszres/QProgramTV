#include "mainwindow.h"
#include "versionchecker.h"
#include <QDebug>
#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    float version = 1.3;
    QString versionURL = "https://raw.githubusercontent.com/lukaszres/QProgramTV/master/version.txt";

    QApplication a(argc, argv);

    VersionChecker vChecker;
    vChecker.setVersion(version);
    vChecker.setFilePath(versionURL);
    if (vChecker.isActual())
        qDebug()<<"Twoja wersja jest aktualna";
    else
        qDebug()<<"Masz nieaktualną wersję";

    MainWindow w;
    w.show();

    return a.exec();
}
