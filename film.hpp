#pragma once
#ifndef FILM_H
#define FILM_H
#include <QString>

class Film{
public:
    Film(QString time, QString channel, QString genre, QString title);
    Film(QString time, QString channel, QString genre, QString title, QString genreSuffix);

    const QString& getGenre() const {
		return genre;
	}
    const QString& getGenreSuffix() const {
        return genreSuffix;
    }

    const QString& getTime() const {
		return time;
	}

    const QString& getTitle() const {
		return title;
	}

    const QString& getChannel() const {
		return channel;
    }

private:
    QString time;
    QString channel;
    QString genre;
    QString genreSuffix;
    QString title;
};
#endif // FILM_H
