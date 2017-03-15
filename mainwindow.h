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
//-Create listview with selectionMode: ExtendedSelection instead comboBox to show channels it can be added to favourites
//-Sometimes, while do some operations on favourite channels(adding/removing) this error ocure:
//                          QSortFilterProxyModel: index from wrong model passed to mapToSource
//-Create adding to all channels editor


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

    void createAndSortAllChannels();
        void generateAllChannels();
        void createAllChannelsFromFile(QFile &file);
    void initFavAndLeftChannels();
        void createFavChannels();
        void showLeftChannels();
            void createFavoritesChannelsFromFile(QFile &file);
        void showSortedFavChannels();
            void sortFavChannels();
            void showFavChannels();
            void removeChannelsFromLeftChannels();
        void setSavedFavChannels();
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
    Channels *channels = new Channels();
    QString textBrowserContent;
    QString currentChannel;

    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    QStringList genreList;
    QStringList genreLeft;
    QStringList genreChoosed;
    QString fileFavouritesChannels = "setup.txt";
    QString fileAllChannels = "channels.txt";
    QVector<QString> allChannels;

    const QString selectChannel = "Wybierz kanał";
    QStringListModel *model;
    QStringList favouriteChannels;
    QStringList savedFavChannels;
    QMessageBox messageBox;
    QMessageBox messageBoxOnLoad;
};

#endif // MAINWINDOW_H
