#pragma once
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    void doDownload(QString channel);
    QString getHtmlContent() const;
    QString getChannel() const;
signals:
    void finished();
public slots:
    void replyFinished (QNetworkReply *reply);
private:
   QNetworkAccessManager *manager;
   QString htmlContent;
   QString channel;
};

#endif // DOWNLOADER_H
