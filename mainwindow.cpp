#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "html.hpp"
#include "time.h"
#include <QFile>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createChannels();
    createComboBoxList();
    setFavoritesChannels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &genre)
{
    filmsByGenre.clear();

    if (genre == "Wszystkie")
        filmsByGenre = films;
    else{
    for (unsigned int i=0; i< films.size(); i++){
            if (films[i].getGenre()==genre)
                filmsByGenre.push_back(films[i]);
        }
    }
    createFilms(filmsByGenre);
    ui->label_2->setText("Wybrano " + QString::number(filmsByGenre.size()) + " " + genre );

    ui->textBrowser->setText(textBrowserContent);
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    disconnect(d, &Downloader::finished, this, 0);
    connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished);
    ui->label_2->setText("Proszę czekać...");
    currentChannel = arg1;
    d->doDownload(currentChannel);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        ui->comboBox_2->setEnabled(false);
        disconnect(d, &Downloader::finished, this, 0);
        connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished_MultiChannel);

        genreList.clear();
        textBrowserContent = "";

        films.clear();

        for (unsigned int i = 0; i< favoriteChannelsList.size(); i++){
            ui->label_2->setText("Proszę czekać...");
            currentChannel = favoriteChannelsList[i];
            d->doDownload(currentChannel);
            QEventLoop loop;
            connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
            connect(this, &MainWindow::finished, &loop, &QEventLoop::quit);
            loop.exec();
        }
        createFilms(films);

    }else{
        ui->comboBox_2->setEnabled(true);
    }
}

void MainWindow::doDownload_Finished()
{
    QString htmlContent = d->getHtmlContent();

    HTML html;
    html.setDate(QString::fromStdString(getDate()));
    html.setChannel(currentChannel);
    films = html.findMarks(htmlContent);
    createFilms(films);

    QString labelText = "Wszystkich filmów: " + QString::number(films.size());
    ui->label->setText(labelText);

    ui->comboBox->clear();
    ui->comboBox->addItem("Wszystkie");
    for (unsigned int i=0; i<genreList.size(); i++){
        ui->comboBox->addItem(genreList[i]);
    }
}

void MainWindow::doDownload_Finished_MultiChannel()
{
    QString htmlContent = d->getHtmlContent();

    HTML html;
    html.setDate(QString::fromStdString(getDate()));
    html.setChannel(currentChannel);
    std::vector <Film> filmsBuf;
    filmsBuf = html.findMarks(htmlContent);
    for (unsigned int i = 0; i<filmsBuf.size(); i++)
    {
        films.push_back(filmsBuf[i]);
    }
    sortByTime(films, 0, films.size() - 1);

    QString labelText = "Wszystkich filmów: " + QString::number(films.size());
    ui->label->setText(labelText);

    ui->comboBox->clear();
    ui->comboBox->addItem("Wszystkie");
    for (unsigned int i=0; i<genreList.size(); i++){
        ui->comboBox->addItem(genreList[i]);
    }

    emit finished();
}

void MainWindow::createFilms(std::vector <Film> films){
    genreList.clear();
    textBrowserContent = "";
    for (unsigned int i = 0; i< films.size(); i++){
        textBrowserContent =  textBrowserContent + films[i].getTime() + " :: " + films[i].getChannel() + " :: "
                + films[i].getGenre() + films[i].getGenreSuffix() + " :: " + films[i].getTitle() + "\n";
        if (std::find(genreList.begin(), genreList.end(), films[i].getGenre()) == genreList.end()){
            genreList.push_back(films[i].getGenre());
        }
    }
    std::sort (genreList.begin(), genreList.end());
}

std::string MainWindow::getDate()
{
    time_t czas;
    struct tm * dateStruct;
    char year[5], month[3], day[3];
    std::string date;
    time( & czas );
    dateStruct = localtime( & czas );
    strftime( year, 5, "%Y", dateStruct );
    strftime( month, 3, "%m", dateStruct );
    if (month[0] == '0'){
        month[0] = month[1];
        month[1] = month[2];
    }
    strftime( day, 3, "%d", dateStruct );
    if (day[0] == '0'){
        day[0] = day[1];
        day[1] = day[2];
    }
    date = year;
    date = date + ',' + month + ',' + day;
    return date;
}

void MainWindow::createChannels()
{
    channelsList.push_back("TVP+1");
    channelsList.push_back("TVP+2");
    channelsList.push_back("Polsat");
    channelsList.push_back("TVN");
    channelsList.push_back("TV+Puls");
    channelsList.push_back("TV4");
    channelsList.push_back("TVN+Siedem");
    channelsList.push_back("Puls+2");
    channelsList.push_back("TV6");
    channelsList.push_back("TV+5+Monde+Europe");
    channelsList.push_back("HBO");
    channelsList.push_back("HBO2");
    channelsList.push_back("HBO+3");
    channelsList.push_back("Cinemax");
    channelsList.push_back("Cinemax+2");
}

int MainWindow::partition (std::vector <Film> & films, int p, int r)
{
    QString x = films[p].getTime();
    int i = p, j = r;
    while(true)
    {
        while (films[j].getTime() > x)
            j--;
        while (films[i].getTime() < x)
            i++;
        if (i<j)
        {
            Film bufor = films[i];
            films[i] = films[j];
            films[j] = bufor;
            i++;
            j--;
        }
        else
            return j;
    }
}

void MainWindow::sortByTime(std::vector <Film> & films, int p, int r)
{
    int q;
    if ( p < r )
    {
        q = partition(films, p, r);
        sortByTime(films, p, q);
        sortByTime(films, q+1, r);
    }
}

void MainWindow::on_pushButtonSave_clicked()
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"Cannot open file";
        return;
    }
    QTextStream out(&file);
    out<<ui->comboBox_3->currentIndex()<<"\n";
    out<<ui->comboBox_4->currentIndex()<<"\n";
    out<<ui->comboBox_5->currentIndex()<<"\n";

    setFavoritesChannels();

    file.flush();
    file.close();
}

void MainWindow::setFavoritesChannels()
{
    QFile file (fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        for (int i=0; i<comboBoxList.size(); i++)
            comboBoxList[i]->setCurrentIndex(i);
    }
    else
    {
        QTextStream in(&file);
        int i = 0;
        while (!in.atEnd())
        {
            comboBoxList[i]->setCurrentIndex(in.readLine().toInt());
            i++;
        }
    }
    favoriteChannelsList.clear();
    favoriteChannelsList.push_back(ui->comboBox_3->currentText());
    favoriteChannelsList.push_back(ui->comboBox_4->currentText());
    favoriteChannelsList.push_back(ui->comboBox_5->currentText());
}

void MainWindow::createComboBoxList ()
{
    comboBoxList.push_back(ui->comboBox_3);
    comboBoxList.push_back(ui->comboBox_4);
    comboBoxList.push_back(ui->comboBox_5);

    for (unsigned int i = 0; i < channelsList.size(); i++){
        ui->comboBox_2->addItem(channelsList[i]);
        for (int j=0; j < comboBoxList.size(); j++)
            comboBoxList[j]->addItem(channelsList[i]);
    }
}
