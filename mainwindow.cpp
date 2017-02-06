#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "html.hpp"
#include "time.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->comboBox_2->addItem("TVP+1");
    ui->comboBox_2->addItem("TVP+2");
    ui->comboBox_2->addItem("Polsat");
    ui->comboBox_2->addItem("TVN");
    ui->comboBox_2->addItem("TV+Puls");
    ui->comboBox_2->addItem("TV4");
    ui->comboBox_2->addItem("TVN+Siedem");
    ui->comboBox_2->addItem("Puls+2");
    ui->comboBox_2->addItem("TV6");
    ui->comboBox_2->addItem("TV+5+Monde+Europe");
    ui->comboBox_2->addItem("HBO");
    ui->comboBox_2->addItem("HBO2");
    ui->comboBox_2->addItem("HBO+3");
    ui->comboBox_2->addItem("Cinemax");
    ui->comboBox_2->addItem("Cinemax+2");
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
    connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished);
    ui->label_2->setText("Proszę czekać...");
    d->doDownload(arg1);
}



void MainWindow::doDownload_Finished()
{
    QString htmlContent = d->getHtmlContent();

    HTML html;
    html.setDate(QString::fromStdString(getDate()));
    html.setChannel(ui->comboBox_2->currentText().toStdString());
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
