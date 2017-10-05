#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include "downloader.h"
#include "film.hpp"
#include "channels.h"

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QFile>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createTextFromFilms(std::vector <Film> films);
    void createAllGenres();
    void showLeftGenres();
    void createComboBoxGenre(QComboBox *combobox);

    QString removeEntity(QString str);
    void sortByTime(std::vector<Film> &films);
    Downloader *d = new Downloader();


    void initFavAndLeftChannels();
        void showLeftChannels();
        void showFavChannels();
            void removeChannelsFromLeftChannels();
        void showNumberOfFavChannelsAndLeftChannels();

    void addChannelsLeft();
        void sortComboBoxChannelLeft();
    void removeChannelsFromFavourite();

    void clearListView();
    void addChannelToFav();

    void saveFavChannelsToFile(QFile &file);

    void addAndSortToGenresChoosed();
    void showChoosedGenres();
    void removeChoosedFromLeftGenres();

    void addAndSortToGenresLeft();
    void removeLeftFromChoosedGenres();

    void createFilmsByGenre();

    void showNumberOfFavAndLeftChannels();

signals:
    void finished();

private slots:
    void doDownload_Finished();


    void on_pushButtonClear_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();

    void on_pushButtonSaveFavourites_clicked();
    void on_pushButtonLoadFavourites_clicked();

    void on_pushButton_Start_clicked();

    void on_pushButton_AddGenres_clicked();
    void on_pushButton_RemoveGenres_clicked();

    void on_pushButton_AddAllGenres_clicked();

    void on_pushButton_RemoveAllGenres_clicked();

private:
    Ui::MainWindow *ui;
    Channels *allChannels = new Channels();
    Channels *favChannels = new Channels();
    Channels *leftChannels = new Channels();
    QString textBrowserContent;
    QString currentChannel;

    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    QStringList genreList;
    QStringList genreLeft;
    QStringList genreChoosed;
    QString fileFavouritesChannels = ":/channels.txt";
    QString fileAllChannels = ":/setup.txt";

    const QString selectChannel = "Wybierz kanał";
    QStringListModel *model;
    QMessageBox messageBox;
    QMessageBox messageBoxOnLoad;
};

#endif // MAINWINDOW_H
