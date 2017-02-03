#include "downloader.h"
#include "mainwindow.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

QString Downloader::doDownload(QString channel)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://www.filmweb.pl/program-tv/" + channel)));
    return htmlContent;
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

QString Downloader::getHtmlContent() const
{
    return htmlContent;
}
