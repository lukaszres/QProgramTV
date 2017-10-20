#pragma once
#ifndef HTTP_H
#define HTTP_H
#include <QDateTime>
//#include "film.hpp"
class Film;

class HTML{
public:

    std::vector<Film> findMarks(QString &htmlContent);
    void removeEntity(QString &arg);

    const QString& getChannel() const {
		return channel;
	}

    void setChannel(const QString & channel) {
		this->channel = channel;
	}

	const std::string& getUrl() const {
		return url;
	}

    void setUrl(const std::string& url) {
        this->url = url;
	}

    void setCommand(const std::string& command) {
        this->command = command;
    }

    const std::string& getCommand() const {
        return command;
    }

    void setFile(const std::string & file) {
        this->file = file;
    }

    const std::string & getFile() const {
        return this->file;
    }

    void setDate (const QString & date) {
        this->date = date;
    }

    const QString & getDate() const {
        return this->date;
    }

    void setQdat (const QDateTime & qdat) {
        this->qdat = qdat;
    }

    QDateTime & getQdat() {
        return this->qdat;
    }

private:
    std::string file;
    QString channel;
    std::string url;
    std::string command;
    QString date;
    QDateTime qdat;
};
#endif // HTTP_H
