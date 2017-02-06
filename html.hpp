#pragma once
#ifndef HTTP_H
#define HTTP_H
#include "film.hpp"

class HTML{
public:

    std::vector<Film> findMarks(QString &htmlContent) const;

    const std::string& getChannel() const {
		return channel;
	}

    void setChannel(const std::string & channel) {
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

private:
    std::string file;
    std::string channel;
    std::string url;
    std::string command;
    QString date;
};
#endif // HTTP_H