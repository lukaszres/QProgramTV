#pragma once
#ifndef FILM_H
#define FILM_H
#include <QDateTime>
#include <QString>

class Film{
public:
    Film(QString m_Channel, QString m_Genre, QString m_Title, QString m_GenreSuffix, QDateTime m_DateTime);

    const QString& getGenre() const {
        return m_Genre;
	}
    const QString& getGenreSuffix() const {
        return m_GenreSuffix;
    }

    const QString& getTitle() const {
        return m_Title;
	}

    const QString& getChannel() const {
        return m_Channel;
    }

    QString getDate() const;
    void setDate(const QString &value);

    QDateTime getDateTime() const {
        return m_DateTime;
    }

private:
    QString m_Channel;
    QString m_Genre;
    QString m_GenreSuffix;
    QString m_Title;
    QDateTime m_DateTime;
};
#endif // FILM_H
