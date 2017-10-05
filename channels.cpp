#include "channels.h"
#include <QtAlgorithms>
#include <QString>
#include <QTextStream>
#include <QDebug>

Channels::Channels(){

}

void Channels::create(){
    QFile file (this->fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        generate();
    }
    else{
        createFromFile(file);
    }
}

void Channels::generate(){
    qDebug()<<"Channels: Cannot open file"<<this->fileName;
    channels.push_back("TVP+1");
    channels.push_back("TVP+2");
}

void Channels::createFromFile(QFile &file){
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        this->channels.push_back(line);
    }
}

void Channels::sort()
{
    std::sort(channels.begin(), channels.end(),
          [](const QString &a, const QString &b){return a.toLower() < b.toLower();});
}

void Channels::setFileName(QString &file){
    this->fileName = file;
}

int Channels::size(){
    return this->channels.size();
}

QString Channels::get(int i)
{
    return channels[i];
}

QStringList Channels::getAll()
{
    return this->channels;
}

void Channels::pushBack(QString channel)
{
    this->channels.push_back(channel);
}

void Channels::clear()
{
    this->channels.clear();
}

void Channels::remove(int i)
{
    this->channels.removeAt(i);
}

void Channels::remove(QString channel)
{
    int i = this->channels.indexOf(channel);
    remove(i);
}

void Channels::setAll(QStringList channels)
{
    this->channels = channels;
}
