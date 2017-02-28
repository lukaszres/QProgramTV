#include "film.hpp"

Film::Film(QString channel, QString genre, QString title, QString genreSuffix, QDateTime dateTime){
    this->channel = channel;
    this->genre = genre;
    this->title = title;
    this->genreSuffix = genreSuffix;
    this->dateTime = dateTime;
}
