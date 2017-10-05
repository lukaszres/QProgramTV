#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "html.hpp"
#include "time.h"
#include "channels.h"
#include <QString>
#include <QVector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    allChannels->setFileName(fileAllChannels);
    favChannels->setFileName(fileFavouritesChannels);

    initFavAndLeftChannels();
//    QObject::connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished);
    QObject::connect(d, SIGNAL(finished()), this, SLOT(doDownload_Finished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Start_clicked()
{
    genreList.clear();
    textBrowserContent.clear();
    films.clear();
    const int size = favChannels->size();

    messageBoxOnLoad.setText("Zakończono pobieranie filmów!");
    messageBoxOnLoad.setWindowTitle("QProgramTV Pobieranie!");
    messageBoxOnLoad.show();

    for (int i = 0; i< size; i++){
        currentChannel = favChannels->get(i);
        ui->label_2->setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.");
        messageBoxOnLoad.setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.\n"
                                 "Wszystkich filmów: " + QString::number(films.size()));
        d->doDownload(currentChannel);
        QEventLoop loop;
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
//        connect(this, &MainWindow::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }
    messageBoxOnLoad.hide();
    messageBox.setText("Zakończono pobieranie filmów!");
    messageBox.setWindowTitle("QProgramTV Ukończono!");
    messageBox.show();
    createTextFromFilms(films);
    createAllGenres();
    genreLeft = genreList;
    showLeftGenres();
    createFilmsByGenre();
}

void MainWindow::createTextFromFilms(std::vector <Film> films){
    textBrowserContent.clear();
    sortByTime(films);
    for (unsigned int i = 0; i< films.size(); i++){
        textBrowserContent =  textBrowserContent + films[i].getDateTime().time().toString("hh:mm") + " :: "
                + removeEntity(films[i].getChannel()) + " :: "
                + films[i].getGenre() + films[i].getGenreSuffix() + " :: " + films[i].getTitle() + "\n";
    }
}

void MainWindow::sortByTime(std::vector <Film> & films)
{
    std::sort(std::begin(films), std::end(films),
              [](const Film& i, const Film& j) { return i.getDateTime() < j.getDateTime(); });
}

QString MainWindow::removeEntity(QString str)
{
    str.replace("+", " ");
    str.replace("%2B", "+");
    return str;
}

void MainWindow::createAllGenres()
{
    genreList.clear();
    for (unsigned int i = 0; i< films.size(); i++){
        if (std::find(genreList.begin(), genreList.end(), films[i].getGenre()) == genreList.end()){
            genreList.push_back(films[i].getGenre());
        }
    }
    std::sort (genreList.begin(), genreList.end());
}

void MainWindow::showLeftGenres()
{
    QStringListModel *modelG = new QStringListModel(this);
    modelG->setStringList(genreLeft);
    ui->listView_LeftGenres->setModel(modelG);
}

void MainWindow::createFilmsByGenre()
{
    filmsByGenre.clear();
    if (genreChoosed.size() == 0)
        filmsByGenre = films;
    else{
    for (unsigned int i=0; i< films.size(); i++){
            if (genreChoosed.contains(films[i].getGenre()))
                filmsByGenre.push_back(films[i]);
        }
    }
    createTextFromFilms(filmsByGenre);
    ui->label_2->setText("Wybrano " + QString::number(filmsByGenre.size()) + " filmów");
    ui->textBrowser->setText(textBrowserContent);
}


void MainWindow::initFavAndLeftChannels()
{
    clearListView();

    allChannels->clear();
    allChannels->create();
    allChannels->sort();

    favChannels->clear();
    favChannels->create();
    favChannels->sort();
    showFavChannels();

    leftChannels->clear();
    leftChannels->setAll(allChannels->getAll());
    removeChannelsFromLeftChannels();
    showLeftChannels();

    showNumberOfFavAndLeftChannels();
}

void MainWindow::showLeftChannels()
{
    ui->comboBoxRemained->clear();
    ui->comboBoxRemained->addItems(leftChannels->getAll());
}

void MainWindow::showFavChannels()
{
    model = new QStringListModel(this);
    model->setStringList(favChannels->getAll());
    ui->listView->setModel(model);
}

void MainWindow::removeChannelsFromLeftChannels()
{
    for (int i=0; i<favChannels->size(); i++)
    {
        leftChannels->remove(favChannels->get(i));
    }
}

void MainWindow::showNumberOfFavAndLeftChannels()
{
    int c = allChannels->size(), f = favChannels->size();
    ui->label_numberOfFavLeftChannels->setText(
                "Wszystkich kanałów: " + QString::number(c) + ", "
                "ulubionych kanałów: " + QString::number(f) + ", "
                "pozostało " + QString::number(c-f) + " kanałów"
                );
}



void MainWindow::on_pushButtonClear_clicked()
{
    for (int i=0; i<favChannels->size(); i++)
    {
        leftChannels->pushBack(favChannels->get(i));
    }
    leftChannels->sort();
    showLeftChannels();
    clearListView();
    favChannels->clear();
    showNumberOfFavAndLeftChannels();
}

void MainWindow::clearListView()
{
    QStringListModel *model;
    model = new QStringListModel(this);
    ui->listView->setModel(model);
}



void MainWindow::on_pushButtonAdd_clicked()
{
    addChannelToFav();
    showFavChannels();
    leftChannels->remove(ui->comboBoxRemained->currentIndex());
    showLeftChannels();
    showNumberOfFavAndLeftChannels();
}

void MainWindow::addChannelToFav()
{
    if (ui->comboBoxRemained->currentText() != "")
    {
        favChannels->pushBack(ui->comboBoxRemained->currentText());
    }
}



void MainWindow::on_pushButtonRemove_clicked()
{
    addChannelsLeft();
    showLeftChannels();
    ui->comboBoxRemained->setCurrentIndex(0);
    removeChannelsFromFavourite();
    showNumberOfFavAndLeftChannels();
}

void MainWindow::addChannelsLeft()
{
    QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
    if (!selected.isEmpty())
    {
        for(int i=0; i<selected.size(); i++)
        {
            leftChannels->pushBack(selected[i].data().toString());
        }
    }
    leftChannels->sort();
}

void MainWindow::sortComboBoxChannelLeft()
{
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(ui->comboBoxRemained);
    proxy->setSourceModel(ui->comboBoxRemained->model());
    ui->comboBoxRemained->model()->setParent(proxy);
    ui->comboBoxRemained->setModel(proxy);
    ui->comboBoxRemained->model()->sort(0);
}

void MainWindow::removeChannelsFromFavourite()
{
    QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
//        qSort(selected);
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
        {
            favChannels->remove(selected.at(i).row()-i);
        }
        ((QStringListModel*) ui->listView->model())->setStringList(favChannels->getAll());
    }
}



void MainWindow::on_pushButtonSaveFavourites_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Zapisywanie", "Czy na pewno zapisać?\nPoprzednia lista ulubionych "
                                                       "kanałów zostanie utracona!",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QFile file(fileFavouritesChannels);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            return;
        }
        else
        {
            saveFavChannelsToFile(file);
        }
    }
}

void MainWindow::saveFavChannelsToFile(QFile &file)
{
    QTextStream out(&file);
    for (int i = 0; i<favChannels->size(); i++)
    {
        out<<favChannels->get(i)<<"\n";
    }
    file.flush();
    file.close();
}



void MainWindow::on_pushButtonLoadFavourites_clicked()
{
    QMessageBox::StandardButton reply;
    reply= QMessageBox::question(this, "Wczytywanie ulubionych kanałów", "Czy na pewno wczytać listę "
                                        "ulubionych kanałów?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        initFavAndLeftChannels();
    }
}



void MainWindow::on_pushButton_AddGenres_clicked()
{
    addAndSortToGenresChoosed();
    showChoosedGenres();
    removeChoosedFromLeftGenres();
    createFilmsByGenre();
}

void MainWindow::addAndSortToGenresChoosed()
{
    QModelIndexList selected = ui->listView_LeftGenres->selectionModel()->selectedIndexes();
    if (!selected.isEmpty())
    {
        for (int i = 0 ; i<selected.size(); i++)
        {
            genreChoosed.push_back(selected[i].data().toString());
        }
    }
    std::sort(genreChoosed.begin(), genreChoosed.end());
}

void MainWindow::showChoosedGenres()
{
    model = new QStringListModel(this);
    model->setStringList(genreChoosed);
    ui->listView_ChoosedGenres->setModel(model);
}

void MainWindow::removeChoosedFromLeftGenres()
{
    QModelIndexList selected = ui->listView_LeftGenres->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
            genreLeft.removeAt(selected.at(i).row()-i);
        ((QStringListModel*) ui->listView_LeftGenres->model())->setStringList(genreLeft);
    }
}



void MainWindow::on_pushButton_RemoveGenres_clicked()
{
    addAndSortToGenresLeft();
    showLeftGenres();
    removeLeftFromChoosedGenres();
    showChoosedGenres();
    createFilmsByGenre();
}

void MainWindow::addAndSortToGenresLeft()
{
    QModelIndexList selected = ui->listView_ChoosedGenres->selectionModel()->selectedIndexes();
    if (!selected.isEmpty())
    {
        for (int i = 0 ; i<selected.size(); i++)
        {
            genreLeft.push_back(selected[i].data().toString());
        }
    }
    std::sort(genreLeft.begin(), genreLeft.end());
}

void MainWindow::removeLeftFromChoosedGenres()
{
    QModelIndexList selected = ui->listView_ChoosedGenres->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
            genreChoosed.removeAt(selected.at(i).row()-i);
    }
}



void MainWindow::on_pushButton_AddAllGenres_clicked()
{
    ui->listView_LeftGenres->selectAll();
    on_pushButton_AddGenres_clicked();
}

void MainWindow::on_pushButton_RemoveAllGenres_clicked()
{
    ui->listView_ChoosedGenres->selectAll();
    on_pushButton_RemoveGenres_clicked();
}


void MainWindow::doDownload_Finished()
{
    QString htmlContent = d->getHtmlContent();
    HTML html;
    for (int i=0; i<2; i++)
    {
        html.setDate(QDateTime::currentDateTime().date().addDays(i).toString("yyyy,M,d"));
        qint8 j = i;
        html.setQdat(QDateTime().currentDateTime().addDays(j));
        html.setChannel(currentChannel);
        std::vector <Film> filmsBuf;
        filmsBuf = html.findMarks(htmlContent);
        for (unsigned int j = 0; j<filmsBuf.size(); j++)
        {
            films.push_back(filmsBuf[j]);
        }
    }
    QString labelText = "Wszystkich filmów: " + QString::number(films.size());
    ui->label->setText(labelText);
    emit finished();
}
