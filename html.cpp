#include <regex>
#include <QRegExp>
#include "html.hpp"
#include <QDebug>

std::vector<Film> HTML::findMarks(QString & htmlContent) const {
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

        QRegExp sx (regSuffix);
        sx.setMinimal(true);
        sx.indexIn(genre);
        genreSuffix = sx.capturedTexts()[0];
        genre.remove(genreSuffix);


        pos += rx.matchedLength();

        Film film(hour, QString::fromStdString(this->channel), genre, title, genreSuffix);
        films.push_back(film);
    }
    return films;
}
