#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloader.h"
#include "film.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createFilms(std::vector <Film> films);

    std::string MainWindow::getDate();
    Downloader *d = new Downloader();

private slots:
    void doDownload_Finished();

    void on_comboBox_currentIndexChanged(const QString &genre);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString textBrowserContent;
    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    std::vector<QString> genreList;
};

#endif // MAINWINDOW_H
