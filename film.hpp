#pragma once
#ifndef FILM_H
#define FILM_H
#include <QString>
#include <QDateTime>

class Film{
public:
    Film(QString channel, QString genre, QString title, QString genreSuffix, QDateTime dateTime);

    const QString& getGenre() const {
		return genre;
	}
    const QString& getGenreSuffix() const {
        return genreSuffix;
    }

    const QString& getTitle() const {
		return title;
	}

    const QString& getChannel() const {
		return channel;
    }

    QString getDate() const;
    void setDate(const QString &value);

    QDateTime getDateTime() const {
        return dateTime;
    }

private:
    QString channel;
    QString genre;
    QString genreSuffix;
    QString title;
    QDateTime dateTime;
};
#endif // FILM_H
