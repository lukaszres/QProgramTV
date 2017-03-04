#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include "downloader.h"
#include "film.hpp"

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QFile>
//mają być najpierw pobierane wszystkie kanały, potem sortowane, potem wyświetlane!
//Zamiast combo stworzyc listview z mozliwoscią multidodawania kanałów do fav.
//podczas sortowania listy fav wyskakuje błąd: QSortFilterProxyModel: index from wrong model passed to mapToSource


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
    void createComboBoxGenre(QComboBox *combobox);

    QString removeEntity(QString str);
    void sortByTime(std::vector<Film> &films);
    Downloader *d = new Downloader();

    void createAndSortAllChannels();
        void generateAllChannels();
        void createAllChannelsFromFile(QFile &file);
    void initFavAndLeftChannels();
        void createFavChannels();
        void createChannelsLeft();
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

signals:
    void finished();

private slots:
    void doDownload_Finished();

    void on_checkBox_toggled(bool checked);

    void on_comboBox_Genre_Main_currentIndexChanged(const QString &genre);
    void on_comboBox_Genre_1_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_2_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_3_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_4_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_5_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_6_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_7_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_8_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &currentChannel);

    void on_pushButtonClear_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();

    void on_pushButtonSaveFavourites_clicked();
    void on_pushButtonLoadFavourites_clicked();

private:
    Ui::MainWindow *ui;
    QString textBrowserContent;
    QString currentChannel;

    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    std::vector<QString> genreList;
    QString fileFavouritesChannels = "setup.txt";
    QString fileAllChannels = "channels.txt";
    QVector<QString> channels;

    const QString selectChannel = "Wybierz kanał";
    QStringListModel *model;
    QStringList favouriteChannels;
    QStringList savedFavChannels;
};

#endif // MAINWINDOW_H
