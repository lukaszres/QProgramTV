#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H
#include <QString>
#include <QObject>
#include <QNetworkReply>

class VersionChecker : public QObject
{
    Q_OBJECT
public:
    VersionChecker();
    float getCurrentVersion();
    float getActualVersion();
    bool isActual();
    void setCurrentVersion(float &curentVersion);
    void setFileURL(QString &filePath);
private:
    float m_CurrentVersion{};
    float m_ActualVersion{};
    QString m_FileURL;
signals:
    void finished();
public slots:
    void replyFinished (QNetworkReply *reply);
};

#endif // VERSIONCHECKER_H
