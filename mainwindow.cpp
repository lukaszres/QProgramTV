#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "html.hpp"
#include "time.h"
#include <QString>
#include <QVector>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createAndSortAllChannels();
    initFavAndLeftChannels();
    connect(d, &Downloader::finished, this, &MainWindow::doDownload_Finished);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_Genre_Main_currentIndexChanged(const QString &genre)
{
    filmsByGenre.clear();
    if (genre == "Wszystkie gatunki")
        filmsByGenre = films;
    else{
    for (unsigned int i=0; i< films.size(); i++){
            if (films[i].getGenre()==genre || films[i].getGenre()==ui->comboBox_Genre_1->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_2->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_3->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_4->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_5->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_6->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_7->currentText()
                    || films[i].getGenre()==ui->comboBox_Genre_8->currentText())
                filmsByGenre.push_back(films[i]);
        }
    }
    createFilms(filmsByGenre);
    ui->label_2->setText("Wybrano " + QString::number(filmsByGenre.size()) + " filmów");
    ui->textBrowser->setText(textBrowserContent);
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &currentChannel)
{
//    if (currentChannel != channels[0].name)
//    {
//        ui->label_2->setText("Proszę czekać...");
//        d->doDownload(currentChannel);
//    }
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        ui->comboBox_2->setEnabled(false);

        genreList.clear();
        textBrowserContent.clear();
        films.clear();
        for (int i = 0; i< favouriteChannels.size(); i++){
            currentChannel = favouriteChannels[i];
            qDebug()<<"on_checkbox"<<currentChannel;
//            if (currentChannel != channels[0].name)
//            {
                ui->label_2->setText("Proszę czekać...");
                d->doDownload(currentChannel);
                QEventLoop loop;
                connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
                connect(this, &MainWindow::finished, &loop, &QEventLoop::quit);
                loop.exec();
//            }
        }
        ui->label_2->setText("Wykonano!");
        createFilms(films);

    }else{
        ui->comboBox_2->setEnabled(true);
    }
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
    createComboBoxGenre(ui->comboBox_Genre_Main);
    createComboBoxGenre(ui->comboBox_Genre_1);
    createComboBoxGenre(ui->comboBox_Genre_2);
    createComboBoxGenre(ui->comboBox_Genre_3);
    createComboBoxGenre(ui->comboBox_Genre_4);
    createComboBoxGenre(ui->comboBox_Genre_5);
    createComboBoxGenre(ui->comboBox_Genre_6);
    createComboBoxGenre(ui->comboBox_Genre_7);
    createComboBoxGenre(ui->comboBox_Genre_8);

    emit finished();
}

void MainWindow::createComboBoxGenre(QComboBox * combobox)
{
    combobox->clear();
    if (combobox == ui->comboBox_Genre_Main)
        combobox->addItem("Wszystkie gatunki");
    else
        combobox->addItem("---Wybierz gatunek---");
    for (unsigned int i=0; i<genreList.size(); i++){
        combobox->addItem(genreList[i]);
    }
}

void MainWindow::createFilms(std::vector <Film> films){
    genreList.clear();
    textBrowserContent.clear();
    sortByTime(films);
    for (unsigned int i = 0; i< films.size(); i++){
        textBrowserContent =  textBrowserContent + films[i].getDateTime().time().toString("hh:mm") + " :: "
//                + films[i].getDateTime().date().toString("MM.dd") + " :: "
                + removeEntity(films[i].getChannel()) + " :: "
                + films[i].getGenre() + films[i].getGenreSuffix() + " :: " + films[i].getTitle() + "\n";
        if (std::find(genreList.begin(), genreList.end(), films[i].getGenre()) == genreList.end()){
            genreList.push_back(films[i].getGenre());
        }
    }
    std::sort (genreList.begin(), genreList.end());
}

QString MainWindow::removeEntity(QString str)
{
    str.replace("+", " ");
    str.replace("%2B", "+");
    return str;
}

void MainWindow::sortByTime(std::vector <Film> & films)
{
    std::sort(std::begin(films), std::end(films),
              [](const Film& i, const Film& j) { return i.getDateTime() < j.getDateTime(); });
}











void MainWindow::createAndSortAllChannels()
{
    QFile file (fileAllChannels);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        generateAllChannels();
    }
    else
    {
        createAllChannelsFromFile(file);
    }
    qSort(channels.begin(), channels.end(),
          [](const QString &a, QString &b){return a.toLower() < b.toLower();});
}

void MainWindow::generateAllChannels()
{
    qDebug()<<"Cannot open file";
    channels.push_back("TVP+1");
    channels.push_back("TVP+2");
}

void MainWindow::createAllChannelsFromFile(QFile &file)
{
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        channels.push_back(line);
    }
}

void MainWindow::initFavAndLeftChannels()
{
    createFavChannels();
    createChannelsLeft();
    showSortedFavChannels();
    removeChannelsFromLeftChannels();
    setSavedFavChannels();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::createChannelsLeft()
{
    ui->comboBoxRemained->clear();
    for (int i=0; i<channels.size(); i++)
    {
        ui->comboBoxRemained->addItem(channels[i]);
    }
}

void MainWindow::createFavChannels()
{
    QFile file (fileFavouritesChannels);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
//        for (int i=0; i<comboBoxes.size(); i++)
//            comboBoxes[i]->setCurrentIndex(i);
    }
    else
    {
        favouriteChannels.clear();
        createFavoritesChannelsFromFile(file);
    }
    file.close();
}

void MainWindow::createFavoritesChannelsFromFile(QFile& file)
{
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(channels.indexOf(line) != -1)
            favouriteChannels.push_back(line);
    }
}

void MainWindow::showSortedFavChannels()
{
    sortFavChannels();
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
    model->setStringList(favouriteChannels);
    ui->listView->setModel(model);
}

void MainWindow::removeChannelsFromLeftChannels()
{
    for (int i=0; i<favouriteChannels.size(); i++)
    {
        ui->comboBoxRemained->removeItem(ui->comboBoxRemained->findText(favouriteChannels[i]));
    }
}

void MainWindow::setSavedFavChannels()
{
    savedFavChannels = favouriteChannels;
}

void MainWindow::showNumberOfFavChannelsAndLeftChannels()
{
    int c = channels.size(), f = favouriteChannels.size();
    ui->label_numberOfFavLeftChannels->setText(
                "Wszystkich kanałów: " + QString::number(c) + ", "
                "ulubionych kanałów: " + QString::number(f) + ", "
                "pozostało " + QString::number(c-f) + " kanałów"
                );
}
















void MainWindow::on_pushButtonClear_clicked()
{
    clearListView();
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
    removeChannelsFromLeftChannels();
    showNumberOfFavChannelsAndLeftChannels();
}

void MainWindow::addChannelToFav()
{
    if (ui->comboBoxRemained->currentText() != "")
        favouriteChannels<<ui->comboBoxRemained->currentText();
}

void MainWindow::on_pushButtonRemove_clicked()
{
    addChannelsLeft();
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
            ui->comboBoxRemained->addItem(selected[i].data().toString());
        }
    }
    sortComboBoxChannelLeft();
    ui->comboBoxRemained->setCurrentIndex(0);
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
            favouriteChannels.removeAt(selected.at(i).row()-i);
        ((QStringListModel*) ui->listView->model())->setStringList(favouriteChannels);
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
        setSavedFavChannels();
    }
}

void MainWindow::saveFavChannelsToFile(QFile &file)
{
    QTextStream out(&file);
    for (int i = 0; i<favouriteChannels.size(); i++)
    {
        out<<favouriteChannels[i]<<"\n";
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






void MainWindow::on_comboBox_Genre_1_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_2_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_3_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}
void MainWindow::on_comboBox_Genre_4_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_5_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_6_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_7_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}

void MainWindow::on_comboBox_Genre_8_currentIndexChanged(const QString &arg1)
{
    on_comboBox_Genre_Main_currentIndexChanged(ui->comboBox_Genre_Main->currentText());
}
