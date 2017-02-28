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
    void createComboBoxGenre(QComboBox *combobox);
    void setFavoritesChannels();
    QString removeEntity(QString str);

    Downloader *d = new Downloader();

signals:
    void finished();

private slots:
    void doDownload_Finished();
    void sortByTime(std::vector<Film> &films);

    void on_checkBox_toggled(bool checked);

    void on_pushButtonSave_clicked();

    void on_comboBox_Genre_Main_currentIndexChanged(const QString &genre);
    void on_comboBox_Genre_1_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_2_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_3_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_4_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_5_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_6_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_7_currentIndexChanged(const QString &arg1);
    void on_comboBox_Genre_8_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButtonReset_clicked();

    void on_pushButtonLoad_clicked();

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
    QString fileChannelsName = "channels.txt";
};

#endif // MAINWINDOW_H
