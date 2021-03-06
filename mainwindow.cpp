#include "downloader.h"
#include "film.hpp"
#include "html.hpp"
#include "channels.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    d = new Downloader{};
    allChannels = new Channels();
    favChannels = new Channels();
    leftChannels = new Channels();
    createTabs();
    allChannels->setFileName(fileAllChannels);
    favChannels->setFileName(fileFavouritesChannels);
    initFavAndLeftChannels();
    createConections();
}

MainWindow::~MainWindow()
{
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
        label_2->setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.");
        messageBoxOnLoad.setText("Proszę czekać... Pozostało " + QString::number(size - i) + " kanałów do pobrania.\n"
                                 "Wszystkich filmów: " + QString::number(films.size()));
        d->doDownload(currentChannel);
        QEventLoop loop;
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
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
    listView_LeftGenres->setModel(modelG);
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
    label_2->setText("Wybrano " + QString::number(filmsByGenre.size()) + " filmów");
    textBrowser->setText(textBrowserContent);
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
    comboBoxRemained->clear();
    comboBoxRemained->addItems(leftChannels->getAll());
}

void MainWindow::showFavChannels()
{
    model = new QStringListModel(this);
    model->setStringList(favChannels->getAll());
    listView->setModel(model);
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
    label_numberOfFavLeftChannels->setText(
                "Wszystkich kanałów: " + QString::number(c) + ", "
                "ulubionych kanałów: " + QString::number(f) + ", "
                "pozostało " + QString::number(c-f) + " kanałów"
                );
}

void MainWindow::createTabs()
{
    QTabWidget *tabWidget = new QTabWidget;
    QWidget *firstTab = createFirstTab();
    QWidget *secondTab = createSecondTab();
    tabWidget->addTab(firstTab, tr("Spis filmów"));
    tabWidget->addTab(secondTab, tr("Ulubione"));
    setCentralWidget(tabWidget);
}

QWidget *MainWindow::createFirstTab()
{
    QWidget *tab = new QWidget;
    QBoxLayout *tabLayout = new QVBoxLayout;

    textBrowser->setMinimumSize(557, 146);
    textBrowser->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard|Qt::LinksAccessibleByMouse|Qt::LinksAccessibleByKeyboard);
    tabLayout->addWidget(textBrowser);

    QGroupBox *groupBox = new QGroupBox;
    groupBox->setMinimumSize(557, 223);
    groupBox->setTitle(tr("Wybierz gatunek i kanał:"));
    QBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->setMargin(0);
    groupBoxLayout->setSpacing(0);

    QWidget *buttonWidget = new QWidget;
    QLayout *buttonWidgetLayout = new QHBoxLayout;
    buttonWidgetLayout->setContentsMargins(0, 0, 0, 0);
    buttonWidgetLayout->setMargin(0);
    buttonWidgetLayout->setSpacing(0);

    pushButton_Start->setMaximumSize(200, 1000);
    buttonWidgetLayout->addWidget(pushButton_Start);
    buttonWidget->setLayout(buttonWidgetLayout);
    groupBoxLayout->addWidget(buttonWidget);

    QWidget *chosenGenre = new QWidget;
    QBoxLayout *chosenGenreLayout = new QHBoxLayout;
    chosenGenreLayout->setMargin(0);
    QGroupBox *leftBox = new QGroupBox(tr("Wybrane gatunki:"));
    QLayout *leftBoxLayout = new QHBoxLayout;
    leftBoxLayout->addWidget(listView_ChoosedGenres);
    leftBox->setLayout(leftBoxLayout);
    chosenGenreLayout->addWidget(leftBox);
    QGroupBox *midleBox = new QGroupBox;
    midleBox->setMaximumSize(200, 10000);
    QLayout *midleBoxLayout = new QVBoxLayout;
    midleBoxLayout->setMargin(0);

    midleBoxLayout->addWidget(pushButton_AddAllGenres);
    midleBoxLayout->addWidget(pushButton_AddGenres);
    midleBoxLayout->addWidget(pushButton_RemoveGenres);
    midleBoxLayout->addWidget(pushButton_RemoveAllGenres);
    midleBox->setLayout(midleBoxLayout);

    chosenGenreLayout->addWidget(midleBox);
    QGroupBox *rightBox = new QGroupBox(tr("Dostępne gatunki:"));
    QLayout *rightBoxLayout = new QHBoxLayout;

    rightBoxLayout->addWidget(listView_LeftGenres);
    rightBox->setLayout(rightBoxLayout);
    chosenGenreLayout->addWidget(rightBox);
    chosenGenre->setLayout(chosenGenreLayout);
    groupBoxLayout->addWidget(chosenGenre);

    groupBoxLayout->addWidget(label_2);

    groupBoxLayout->addWidget(label);

    groupBox->setLayout(groupBoxLayout);
    tabLayout->addWidget(groupBox);

    tab->setLayout(tabLayout);
    return tab;
}

QWidget *MainWindow::createSecondTab()
{
    QWidget *tab2 = new QWidget;

    QLayout *tab2Layout = new QVBoxLayout;
    QWidget *tab2TopWidget = new QWidget;
    tab2TopWidget->setMaximumSize(10000, 300);
    QLayout *tab2TopWidgetLayout = new QHBoxLayout;
    listView->setMaximumSize(400, 300);
    QWidget *tab2TopWidgetRightWidget = new QWidget;
    QLayout *tab2TopWidgetRightWidgetLayout = new QVBoxLayout;

    tab2TopWidgetRightWidgetLayout->addWidget(pushButtonRemove);
    tab2TopWidgetRightWidgetLayout->addWidget(pushButtonAdd);
    tab2TopWidgetRightWidgetLayout->addWidget(pushButtonClear);
    tab2TopWidgetRightWidgetLayout->addWidget(comboBoxRemained);
    tab2TopWidgetRightWidget->setLayout(tab2TopWidgetRightWidgetLayout);
    tab2TopWidgetLayout->addWidget(listView);
    tab2TopWidgetLayout->addWidget(tab2TopWidgetRightWidget);
    tab2TopWidget->setLayout(tab2TopWidgetLayout);
    QWidget *tab_5MiddleWidget = new QWidget;
    tab_5MiddleWidget->setMaximumSize(10000, 65);
    QLayout *tab_5MiddleWidgetLayout = new QHBoxLayout;
    tab_5MiddleWidgetLayout->setMargin(0);
    tab_5MiddleWidgetLayout->setContentsMargins(0, 0, 0, 0);

    tab_5MiddleWidgetLayout->addWidget(pushButtonSaveFavourites);
    tab_5MiddleWidgetLayout->addWidget(pushButtonLoadFavourites);
    tab_5MiddleWidget->setLayout(tab_5MiddleWidgetLayout);
    QWidget *tab_5BottomWidget = new QWidget;
    QLayout *tab_5BottomWidgetLayout = new QHBoxLayout;

    tab_5BottomWidgetLayout->addWidget(label_numberOfFavLeftChannels);
    tab_5BottomWidget->setLayout(tab_5BottomWidgetLayout);

    tab2Layout->addWidget(tab2TopWidget);
    tab2Layout->addWidget(tab_5MiddleWidget);
    tab2Layout->addWidget(tab_5BottomWidget);
    tab2->setLayout(tab2Layout);
    return tab2;
}

void MainWindow::createConections()
{

    QObject::connect(d, SIGNAL(finished()), this, SLOT(doDownload_Finished()));
    QObject::connect(this->pushButton_Start, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_Start_clicked()));
    QObject::connect(this->pushButton_AddAllGenres, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_AddAllGenres_clicked()));
    QObject::connect(this->pushButton_AddGenres, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_AddGenres_clicked()));
    QObject::connect(this->pushButton_RemoveGenres, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_RemoveGenres_clicked()));
    QObject::connect(this->pushButton_RemoveAllGenres, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_RemoveAllGenres_clicked()));
    QObject::connect(this->pushButtonRemove, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonRemove_clicked()));
    QObject::connect(this->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonAdd_clicked()));
    QObject::connect(this->pushButtonClear, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonClear_clicked()));
    QObject::connect(this->pushButtonSaveFavourites, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonSaveFavourites_clicked()));
    QObject::connect(this->pushButtonLoadFavourites, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonLoadFavourites_clicked()));
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
    listView->setModel(model);
}

void MainWindow::on_pushButtonAdd_clicked()
{
    addChannelToFav();
    showFavChannels();
    leftChannels->remove(comboBoxRemained->currentIndex());
    showLeftChannels();
    showNumberOfFavAndLeftChannels();
}

void MainWindow::addChannelToFav()
{
    if (comboBoxRemained->currentText() != "")
    {
        favChannels->pushBack(comboBoxRemained->currentText());
    }
}

void MainWindow::on_pushButtonRemove_clicked()
{
    addChannelsLeft();
    showLeftChannels();
    comboBoxRemained->setCurrentIndex(0);
    removeChannelsFromFavourite();
    showNumberOfFavAndLeftChannels();
}

void MainWindow::addChannelsLeft()
{
    QModelIndexList selected = listView->selectionModel()->selectedIndexes();
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
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(comboBoxRemained);
    proxy->setSourceModel(comboBoxRemained->model());
    comboBoxRemained->model()->setParent(proxy);
    comboBoxRemained->setModel(proxy);
    comboBoxRemained->model()->sort(0);
}

void MainWindow::removeChannelsFromFavourite()
{
    QModelIndexList selected = listView->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
        {
            favChannels->remove(selected.at(i).row()-i);
        }
        ((QStringListModel*) listView->model())->setStringList(favChannels->getAll());
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
    QModelIndexList selected = listView_LeftGenres->selectionModel()->selectedIndexes();
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
    listView_ChoosedGenres->setModel(model);
}

void MainWindow::removeChoosedFromLeftGenres()
{
    QModelIndexList selected = listView_LeftGenres->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
            genreLeft.removeAt(selected.at(i).row()-i);
        ((QStringListModel*) listView_LeftGenres->model())->setStringList(genreLeft);
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
    QModelIndexList selected = listView_ChoosedGenres->selectionModel()->selectedIndexes();
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
    QModelIndexList selected = listView_ChoosedGenres->selectionModel()->selectedIndexes();
    if(!selected.isEmpty())
    {
        std::sort(selected.begin(), selected.end());
        for (int i=0; i<selected.size(); i++)
            genreChoosed.removeAt(selected.at(i).row()-i);
    }
}



void MainWindow::on_pushButton_AddAllGenres_clicked()
{

    listView_LeftGenres->selectAll();
    on_pushButton_AddGenres_clicked();
}

void MainWindow::on_pushButton_RemoveAllGenres_clicked()
{
    listView_ChoosedGenres->selectAll();
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
    label->setText(labelText);
    emit finished();
}
