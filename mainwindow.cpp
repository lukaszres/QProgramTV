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

    channels->setFileAllChannels(fileAllChannels);
    channels->setFileFavouritesChannels(fileFavouritesChannels);
    channels->createAndSortAllChannels();

    initFavAndLeftChannels();
    connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished);

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
    const int size = channels->favouriteChannelsSize();

    messageBoxOnLoad.setText("Zakończono pobieranie filmów!");
    messageBoxOnLoad.setWindowTitle("QProgramTV Pobieranie!");
    messageBoxOnLoad.show();

    for (int i = 0; i< size; i++){
        qDebug()<<"pętla";
        currentChannel = channels->getChannellFromFavouriteChannels(i);
        ui->label_2->setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.");
        messageBoxOnLoad.setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.\n"
                                 "Wszystkich filmów: " + QString::number(films.size()));
        d->doDownload(currentChannel);
        QEventLoop loop;
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(this, &MainWindow::finished, &loop, &QEventLoop::quit);
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
    channels->createFavChannels();
    channels->setLeftChannelsToAll();
    showSortedFavChannels();
    removeChannelsFromLeftChannels();
    showLeftChannels();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::showLeftChannels()
{
    ui->comboBoxRemained->clear();
    ui->comboBoxRemained->addItems(channels->getLeftChannels());
}

void MainWindow::showSortedFavChannels()
{
    channels->sortFavChannels();
    showFavChannels();
}

void MainWindow::sortFavChannels()
{
    qSort(favouriteChannels.begin(), favouriteChannels.end(),
          [](const QString &a, const QString &b){return a.toLower() < b.toLower();});
}

void MainWindow::showFavChannels()
{
    model = new QStringListModel(this);
    model->setStringList(channels->getFavouriteChannels());
    ui->listView->setModel(model);
}

void MainWindow::removeChannelsFromLeftChannels()
{

    for (int i=0; i<channels->favouriteChannelsSize(); i++)
    {
        channels->removeFromLeftChannels(channels->getChannellFromFavouriteChannels(i));
    }
}

void MainWindow::showNumberOfFavChannelsAndLeftChannels()
{
    int c = channels->allChannelsSize(), f = channels->favouriteChannelsSize();
    ui->label_numberOfFavLeftChannels->setText(
                "Wszystkich kanałów: " + QString::number(c) + ", "
                "ulubionych kanałów: " + QString::number(f) + ", "
                "pozostało " + QString::number(c-f) + " kanałów"
                );
}



void MainWindow::on_pushButtonClear_clicked()
{
    for (int i=0; i<channels->favouriteChannelsSize(); i++)
    {
        channels->leftChannelsPushBack(channels->getChannellFromFavouriteChannels(i));
    }
    channels->sortLeftChannels();
    showLeftChannels();
    clearListView();
    channels->favouriteChannelsClear();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::clearListView()
{
    QStringListModel *model;
    model = new QStringListModel(this);
    ui->listView->setModel(model);

    ui->comboBoxRemained->addItems(favouriteChannels);
    sortComboBoxChannelLeft();
    favouriteChannels.clear();
}



void MainWindow::on_pushButtonAdd_clicked()
{
    addChannelToFav();
    showSortedFavChannels();
    channels->removeFromLeftChannels(ui->comboBoxRemained->currentIndex());
    showLeftChannels();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::addChannelToFav()
{
    if (ui->comboBoxRemained->currentText() != "")
    {
        channels->favouriteChannelsPushBack(ui->comboBoxRemained->currentText());
    }
}



void MainWindow::on_pushButtonRemove_clicked()
{
    addChannelsLeft();
    showLeftChannels();
    ui->comboBoxRemained->setCurrentIndex(0);
    removeChannelsFromFavourite();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::addChannelsLeft()
{
    QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
    if (!selected.isEmpty())
    {
        for(int i=0; i<selected.size(); i++)
        {
            channels->leftChannelsPushBack(selected[i].data().toString());
        }
    }
    channels->sortLeftChannels();
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
        qSort(selected);
        for (int i=0; i<selected.size(); i++)
        {
            channels->removeFromFavouritesChannels(selected.at(i).row()-i);
        }
        ((QStringListModel*) ui->listView->model())->setStringList(channels->getFavouriteChannels());
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
            qDebug()<<"Cannot open file";
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
    for (int i = 0; i<channels->favouriteChannelsSize(); i++)
    {
        out<<channels->getChannellFromFavouriteChannels(i)<<"\n";
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
        clearListView();
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
    qSort(genreChoosed.begin(), genreChoosed.end());
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
        qSort(selected);
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
    qSort(genreLeft.begin(), genreLeft.end());
}

void MainWindow::removeLeftFromChoosedGenres()
{
    QModelIndexList selected = ui->listView_ChoosedGenres->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        qSort(selected);
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
        html.setQdat(QDateTime::QDateTime().currentDateTime().addDays(j));
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
