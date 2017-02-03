#include <iostream>
#include <string>
#include <process.h>
#include <windows.h>
#include <fstream>
#include <regex>
#include "film.hpp"
#include "html.hpp"
#include <QDebug>

std::vector<Film> HTML::findMarks(QString & htmlContent) const {
	std::vector<Film> films;
    std::string regular = "<div class=\"[^>]*\" data-type=\"[^>]*\" data-start=\"" + this->getDate() + ",[^>]*>(.*?)</div>";
    std::regex pattern(regular);
    std::smatch result;
    for (unsigned int i = 0; i < 1; i++) {

        std::string buffor = htmlContent.toStdString();

		auto words_begin = std::sregex_iterator(buffor.begin(), buffor.end(),
				pattern);
        auto words_end = std::sregex_iterator();

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {

			std::smatch match = *i;
			std::string match_str = match.str();

			buffor.erase(0, buffor.find(match_str));

			std::regex znacznik("<[^>]*>");
			std::regex regHour("[0-9]{2}:[0-9]{2}");
			std::regex regTitle("<span class=\"sd\">(.*?)</span>");
			std::regex regGenre("<span class=\"st\">(.*?)</span>");
			std::smatch wynik;
            std::string godzina, title, genre;
			if (std::regex_search(buffor, wynik, regHour)) {
                godzina = wynik[0];
			}
			if (regex_search(buffor, wynik, regGenre)) {
                genre = wynik[0];
			}
			if (regex_search(buffor, wynik, regTitle)) {
                title = wynik[0];
			}
			genre = std::regex_replace(genre, znacznik, "");
			title = std::regex_replace(title, znacznik, "");
			unsigned int position = title.find(genre);
			title.erase(position, title.size());
            Film film(QString::fromStdString(godzina), QString::fromStdString(this->channel), QString::fromStdString(genre), QString::fromStdString(title));
			films.push_back(film);
		}
    }
    return films;
}
