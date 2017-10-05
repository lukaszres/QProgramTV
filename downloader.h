#pragma once
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QDebug>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void doDownload(QString channel);

    QString getHtmlContent() const;

    QString getChannel() const;

//signals:
    void finished();

public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;
   QString htmlContent;
   QString channel;
};

#endif // DOWNLOADER_H
