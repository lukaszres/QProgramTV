#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Film;
#include "downloader.h"
#include <QFile>
#include "channels.h"
#include <QStringListModel>
#include <QMessageBox>
#include <QLabel>
#include <QListView>
#include <QTextBrowser>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
//class Downloader;
//class QComboBox;

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
    Downloader *d = new Downloader{};

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
    void createTabs();
    QWidget *createMainTab();
    QWidget *createSecondTab();

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

    QPushButton *pushButton_Start = new QPushButton(tr("Start"));
    QPushButton *pushButton_AddAllGenres = new QPushButton(tr("<-- wszystkie"));
    QPushButton *pushButton_AddGenres = new QPushButton(tr("<--"));
    QPushButton *pushButton_RemoveGenres = new QPushButton(tr("-->"));
    QPushButton *pushButton_RemoveAllGenres = new QPushButton(tr("wszystkie -->"));
    QPushButton *pushButtonRemove = new QPushButton(tr("Usuń"));
    QPushButton *pushButtonAdd = new QPushButton(tr("Dodaj"));
    QPushButton *pushButtonClear = new QPushButton(tr("Wyczyść"));
    QPushButton *pushButtonSaveFavourites = new QPushButton(tr("Zapisz"));
    QPushButton *pushButtonLoadFavourites = new QPushButton(tr("Wczytaj"));

    QLabel *label_2 = new QLabel;
    QListView *listView_LeftGenres = new QListView;
    QListView *listView_ChoosedGenres = new QListView;
    QTextEdit *textBrowser = new QTextEdit;
    QComboBox *comboBoxRemained = new QComboBox;
    QListView *listView = new QListView;
    QLabel *label_numberOfFavLeftChannels = new QLabel;
    QLabel *label = new QLabel (tr("Wszystkich filmów:"));
};

#endif // MAINWINDOW_H
