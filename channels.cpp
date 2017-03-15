#include "channels.h"
#include <QtAlgorithms>
#include <QString>
#include <QTextStream>
#include <QDebug>

Channels::Channels(){

}

void Channels::createAndSortAllChannels(){
    QFile file (fileAllChannels);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        generateAllChannels();
    }
    else{
        createAllChannelsFromFile(file);
    }
    qSort(allChannels.begin(), allChannels.end(),
          [](const QString &a, QString &b){return a.toLower() < b.toLower();});
}
void Channels::generateAllChannels(){
    qDebug()<<"Channels: Cannot open file";
    allChannels.push_back("TVP+1");
    allChannels.push_back("TVP+2");
}

void Channels::createAllChannelsFromFile(QFile &file){
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        allChannels.push_back(line);
    }
}

void Channels::createFavChannels(){
    QFile file (fileFavouritesChannels);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        favouriteChannels.clear();
        createFavoritesChannelsFromFile(file);
    }
    file.close();
}

void Channels::createFavoritesChannelsFromFile(QFile &file){
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(allChannels.indexOf(line) != -1)
            favouriteChannels.push_back(line);
    }
}

void Channels::sortFavChannels()
{
    qSort(favouriteChannels.begin(), favouriteChannels.end(),
          [](const QString &a, const QString &b){return a.toLower() < b.toLower();});
}

void Channels::sortLeftChannels()
{
    qSort(leftChannels.begin(), leftChannels.end(),
          [](const QString &a, const QString &b){return a.toLower() < b.toLower();});
}


void Channels::setFileAllChannels(QString &file){
    this->fileAllChannels = file;
}

void Channels::setFileFavouritesChannels(QString &file){
    this->fileFavouritesChannels = file;
}

int Channels::allChannelsSize(){
    return this->allChannels.size();
}

int Channels::favouriteChannelsSize()
{
    return favouriteChannels.size();
}

QString Channels::getChannelFromAllChannels(int i)
{
    return allChannels[i];
}

QString Channels::getChannellFromFavouriteChannels(int i)
{
    return favouriteChannels[i];
}

QStringList Channels::getFavouriteChannels()
{
    return this->favouriteChannels;
}

QStringList Channels::getLeftChannels()
{
    return this->leftChannels;
}

void Channels::setLeftChannelsToAll()
{
    leftChannels.clear();
    for (int i=0; i<this->allChannelsSize(); i++)
    {
        leftChannels.push_back(this->allChannels[i]);
    }
}

void Channels::favouriteChannelsPushBack(QString channel)
{
    this->favouriteChannels.push_back(channel);
}

void Channels::favouriteChannelsClear()
{
    this->favouriteChannels.clear();
}

void Channels::leftChannelsPushBack(QString channel)
{
    this->leftChannels.push_back(channel);
}

void Channels::removeFromFavouritesChannels(int i)
{
    this->favouriteChannels.removeAt(i);
}

int Channels::leftChannelsSize()
{
    return this->leftChannels.size();
}

void Channels::removeFromLeftChannels(int i)
{
    this->leftChannels.removeAt(i);
}

void Channels::removeFromLeftChannels(QString channel)
{
    int i = this->leftChannels.indexOf(channel);
    removeFromLeftChannels(i);
}
