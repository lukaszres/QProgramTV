#include "versionchecker.h"
#include <QFile>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QEventLoop>

VersionChecker::VersionChecker()
{

}

QString getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __unix || __unix__
    return "Unix";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #else
    return "Other";
    #endif
}

float VersionChecker::getCurrentVersion()
{
    return m_CurrentVersion;
}

float VersionChecker::getActualVersion()
{
    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(m_FilePath)));

    QEventLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    qDebug()<<getOsName();

return m_ActualVersion;
}

void VersionChecker::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
        QString line = reply->readAll();
        m_ActualVersion = line.toFloat();
    }
    emit finished();
}

bool VersionChecker::isActual()
{
    return m_CurrentVersion == getActualVersion();
}

void VersionChecker::setVersion(float &currentVersion)
{
    m_CurrentVersion = currentVersion;
}

void VersionChecker::setFilePath(QString &filePath)
{
    m_FilePath = filePath;
}
