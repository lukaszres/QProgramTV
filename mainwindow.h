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
    void createChannels();
    void createFilms(std::vector <Film> films);
    void setFavoritesChannels();

    std::string MainWindow::getDate();
    Downloader *d = new Downloader();

signals:
    void finished();

private slots:
    void doDownload_Finished();
    void doDownload_Finished_MultiChannel();
    void sortByTime(std::vector<Film> &films, int p, int r);
    int partition(std::vector<Film> &films, int p, int r);

    void on_comboBox_currentIndexChanged(const QString &genre);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString textBrowserContent;
    QString currentChannel;
    std::vector<QString> channelsList;
    std::vector<QString> favoriteChannelsList;
    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    std::vector<QString> genreList;
    QString fileName = "setup.txt";
};

#endif // MAINWINDOW_H
