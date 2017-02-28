#include <regex>
#include <QRegExp>
#include "html.hpp"
#include <QDebug>

std::vector<Film> HTML::findMarks(QString & htmlContent)
{
    std::vector<Film> films;
    QString regular = "<div class=\"[^>]*\" data-type=\"[^>]*\" data-start=\"" + this->getDate() + ",[^>]*>";
    QString tags("<[^>]*>");
    QString regHour("[0-9]{2}:[0-9]{2}");
    QString regTitle("<span class=\"sd\">(.*)?</span>");
    QString regGenre("<span class=\"st\">(.*)?</span>");
    QString regSuffix(" odc\\.(.*)$");
    QString hour = "", title = "", genre = "", genreSuffix = "";

    QRegExp rx(regular);
    int pos = 0;

    while ((pos = rx.indexIn(htmlContent, pos)) != -1) {

        QRegExp hx (regHour);
        hx.setMinimal(true);
        hx.indexIn(htmlContent, pos);
        hour = hx.capturedTexts()[0];

        QRegExp gx (regGenre);
        gx.setMinimal(true);
        gx.indexIn(htmlContent, pos);
        genre = gx.capturedTexts()[0];
        genre.remove(QRegExp(tags));
        removeEntity(genre);

        QRegExp tx (regTitle);
        tx.setMinimal(true);
        tx.indexIn(htmlContent, pos);
        title = tx.capturedTexts()[0];
        title.remove(QRegExp(tags));
        title.remove(genre);
        while(title[title.size()-1] == ' ')
            title.chop(1);
        while(title[0] == ' ')
            title.remove(0, 1);
        removeEntity(title);

        QRegExp sx (regSuffix);
        sx.setMinimal(true);
        sx.indexIn(genre);
        genreSuffix = sx.capturedTexts()[0];
        genre.remove(genreSuffix);
        removeEntity(genreSuffix);


        pos += rx.matchedLength();

        QDateTime dateTime = getQdat();
        dateTime.setTime(QTime::fromString(hour, "hh:mm"));

        Film film(channel, genre, title, genreSuffix, dateTime);
        films.push_back(film);
    }
    return films;
}

void HTML::removeEntity(QString& arg)
{
    arg.replace("&oacute;", "ó");
    arg.replace("&uuml;", "ü");
    arg.replace("&quot;", "\"");
    arg.replace("&amp;", "&");
    arg.replace("&Eacute;", "É");
    arg.replace("&Ccedil;", "Ç");
    arg.replace("&ouml;", "ö");
    arg.replace("&szlig;", "ß");
    arg.replace("&uacute;", "ú");
    arg.replace("&eacute;", "é");
}
