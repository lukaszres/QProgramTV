#include "film.hpp"

Film::Film(QString time, QString channel, QString genre, QString title){
	this->time = time;
	this->channel = channel;
	this->genre = genre;
	this->title = title;
}

Film::Film(QString time, QString channel, QString genre, QString title, QString genreSuffix){
    this->time = time;
    this->channel = channel;
    this->genre = genre;
    this->title = title;
    this->genreSuffix = genreSuffix;
}
