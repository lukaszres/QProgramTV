#include <QDateTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include "downloader.h"
#include "mainwindow.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

void Downloader::doDownload(QString chanel)
{

    manager = new QNetworkAccessManager(this);

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://www.filmweb.pl/program-tv/" + chanel)));
}

void Downloader::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
        htmlContent = reply->readAll();
    }

    reply->deleteLater();
    emit finished();
}

QString Downloader::getChannel() const
{
    return channel;
}

QString Downloader::getHtmlContent() const
{
    return htmlContent;
}
