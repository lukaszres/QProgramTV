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
    void on_pushButton_clicked();
    void on_doDownload_Finished();

    void on_comboBox_currentIndexChanged(const QString &genre);

private:
    Ui::MainWindow *ui;
    QString textBrowserContent;
    std::vector<Film> films;
    std::vector <Film> filmsByGenre;
    std::vector<QString> genreList;
};

#endif // MAINWINDOW_H
