#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloader.h"
#include "html.hpp"
#include "time.h"
#include <QFile>
#include <QVector>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createChannels();
    createComboBoxList();
    setFavoritesChannels();
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

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    currentChannel = arg1;
    if (currentChannel != channelsList[0])
    {
        ui->label_2->setText("Proszę czekać...");

        d->doDownload(currentChannel);
    }
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        ui->comboBox_2->setEnabled(false);

        genreList.clear();
        textBrowserContent.clear();
        films.clear();

        for (unsigned int i = 0; i< favoriteChannelsList.size(); i++){
            currentChannel = favoriteChannelsList[i];
            if (currentChannel != channelsList[0])
            {
                ui->label_2->setText("Proszę czekać...");

                d->doDownload(currentChannel);
                QEventLoop loop;
                connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
                connect(this, &MainWindow::finished, &loop, &QEventLoop::quit);
                loop.exec();
            }
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
        for (unsigned int i = 0; i<filmsBuf.size(); i++)
        {
            films.push_back(filmsBuf[i]);
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

void MainWindow::createFilms(std::vector <Film> films){
    genreList.clear();
    textBrowserContent.clear();
    sortByTime(films);
    for (unsigned int i = 0; i< films.size(); i++){
        textBrowserContent =  textBrowserContent + films[i].getDateTime().time().toString("hh:mm") + " :: "
                + films[i].getDateTime().date().toString("MM.dd") + " :: "
                + removeEntity(films[i].getChannel()) + " :: "
                + films[i].getGenre() + films[i].getGenreSuffix() + " :: " + films[i].getTitle() + "\n";
        if (std::find(genreList.begin(), genreList.end(), films[i].getGenre()) == genreList.end()){
            genreList.push_back(films[i].getGenre());
        }
    }
    std::sort (genreList.begin(), genreList.end());
}

void MainWindow::createChannels()
{
    channelsList.push_back("Wybierz kanał");
    QFile file (fileChannelsName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"Cannot open file";
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
    else
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            channelsList.push_back(in.readLine());
        }
    }
    std::sort(channelsList.begin() + 1, channelsList.end());
}

void MainWindow::sortByTime(std::vector <Film> & films)
{
    std::sort(std::begin(films), std::end(films),
              [](const Film& i, const Film& j) { return i.getDateTime() < j.getDateTime(); });
}

void MainWindow::on_pushButtonSave_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Zapisywanie", "Czy na pewno zapisać?\nPoprzednia lista ulubionych "
                                                       "kanałów zostanie utracona!",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug()<<"Cannot open file";
            return;
        }
        QTextStream out(&file);
        for (int i = 0; i<comboBoxList.size(); i++)
        {
            out<<comboBoxList[i]->currentText()<<"\n";
        }

        setFavoritesChannels();

        file.flush();
        file.close();
    }
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
            QString line = in.readLine();
            comboBoxList[i]->setCurrentText(line);
            i++;
        }
    }
    file.close();
    favoriteChannelsList.clear();
    for (int i = 0; i<comboBoxList.size(); i++)
    {
        favoriteChannelsList.push_back(comboBoxList[i]->currentText());
    }
}

void MainWindow::createComboBoxList ()
{
    comboBoxList.push_back(ui->comboBox_3);
    comboBoxList.push_back(ui->comboBox_4);
    comboBoxList.push_back(ui->comboBox_5);
    comboBoxList.push_back(ui->comboBox_6);
    comboBoxList.push_back(ui->comboBox_7);
    comboBoxList.push_back(ui->comboBox_8);
    comboBoxList.push_back(ui->comboBox_9);
    comboBoxList.push_back(ui->comboBox_10);
    comboBoxList.push_back(ui->comboBox_11);
    comboBoxList.push_back(ui->comboBox_12);
    comboBoxList.push_back(ui->comboBox_13);
    comboBoxList.push_back(ui->comboBox_14);
    comboBoxList.push_back(ui->comboBox_15);
    comboBoxList.push_back(ui->comboBox_16);
    comboBoxList.push_back(ui->comboBox_17);
    comboBoxList.push_back(ui->comboBox_18);
    comboBoxList.push_back(ui->comboBox_19);
    comboBoxList.push_back(ui->comboBox_20);
    comboBoxList.push_back(ui->comboBox_21);
    comboBoxList.push_back(ui->comboBox_22);
    comboBoxList.push_back(ui->comboBox_23);
    comboBoxList.push_back(ui->comboBox_24);
    comboBoxList.push_back(ui->comboBox_25);
    comboBoxList.push_back(ui->comboBox_26);
    comboBoxList.push_back(ui->comboBox_27);
    comboBoxList.push_back(ui->comboBox_28);
    comboBoxList.push_back(ui->comboBox_29);
    comboBoxList.push_back(ui->comboBox_30);
    comboBoxList.push_back(ui->comboBox_31);
    comboBoxList.push_back(ui->comboBox_32);
    comboBoxList.push_back(ui->comboBox_33);
    comboBoxList.push_back(ui->comboBox_34);
    comboBoxList.push_back(ui->comboBox_35);
    comboBoxList.push_back(ui->comboBox_36);
    comboBoxList.push_back(ui->comboBox_37);
    comboBoxList.push_back(ui->comboBox_38);
    comboBoxList.push_back(ui->comboBox_39);
    comboBoxList.push_back(ui->comboBox_40);
    comboBoxList.push_back(ui->comboBox_41);
    comboBoxList.push_back(ui->comboBox_42);
    comboBoxList.push_back(ui->comboBox_43);
    comboBoxList.push_back(ui->comboBox_44);
    comboBoxList.push_back(ui->comboBox_45);
    comboBoxList.push_back(ui->comboBox_46);
    comboBoxList.push_back(ui->comboBox_47);
    comboBoxList.push_back(ui->comboBox_48);
    comboBoxList.push_back(ui->comboBox_49);
    comboBoxList.push_back(ui->comboBox_50);
    comboBoxList.push_back(ui->comboBox_51);
    comboBoxList.push_back(ui->comboBox_52);
    comboBoxList.push_back(ui->comboBox_53);
    comboBoxList.push_back(ui->comboBox_54);
    comboBoxList.push_back(ui->comboBox_55);
    comboBoxList.push_back(ui->comboBox_56);
    comboBoxList.push_back(ui->comboBox_57);
    comboBoxList.push_back(ui->comboBox_58);
    comboBoxList.push_back(ui->comboBox_59);
    comboBoxList.push_back(ui->comboBox_60);
    comboBoxList.push_back(ui->comboBox_61);

    for (unsigned int i = 0; i < channelsList.size(); i++)
        ui->comboBox_2->addItem(channelsList[i]);

    for (int j=0; j < comboBoxList.size(); j++)
        for (unsigned int i = 0; i < channelsList.size(); i++)
            comboBoxList[j]->addItem(channelsList[i]);
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

QString MainWindow::removeEntity(QString str)
{
    str.replace("%2B", "+");

    return str;
}

void MainWindow::on_pushButtonReset_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Resetowanie ulubionych kanałów", "Usuwa wszystkie kanały z ulubionych.\n"
               "Możliwe będzie później wczytanie wcześniejszej listy ulubionych",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes){
        for (int i=0; i<comboBoxList.size(); i++)
            comboBoxList[i]->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButtonLoad_clicked()
{
    QMessageBox::StandardButton reply;
    reply= QMessageBox::question(this, "Wczytywanie ulubionych kanałów", "Czy na pewno wczytać listę "
                                        "ulubionych kanałów?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        setFavoritesChannels();
}
