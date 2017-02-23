#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
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
    void createComboBoxList ();
    void setFavoritesChannels();

    std::string MainWindow::getDate();
    Downloader *d = new Downloader();

signals:
    void finished();

private slots:
    void doDownload_Finished(bool multiChannel);
    void sortByTime(std::vector<Film> &films);

    void on_comboBox_currentIndexChanged(const QString &genre);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

    void on_pushButtonSave_clicked();

private:
    Ui::MainWindow *ui;
    QString textBrowserContent;
    QString currentChannel;
    std::vector<QString> channelsList;
    QVector <QComboBox*> comboBoxList;
    std::vector<QString> favoriteChannelsList;
    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    std::vector<QString> genreList;
    QString fileName = "setup.txt";
};

#endif // MAINWINDOW_H
