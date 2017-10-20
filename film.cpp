#include "film.hpp"

Film::Film(QString channel, QString genre, QString title, QString genreSuffix, QDateTime dateTime){
    m_Channel = channel;
    m_Genre = genre;
    m_Title = title;
    m_GenreSuffix = genreSuffix;
    m_DateTime = dateTime;
}
