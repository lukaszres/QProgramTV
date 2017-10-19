#include "channels.h"
#include <QtAlgorithms>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QFile>


Channels::Channels(){
    m_Channels = new QStringList();
    m_FileName = new QString;
}

void Channels::create(){
    QFile file (*m_FileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        generate();
    }
    else{
        createFromFile(file);
    }
}

void Channels::generate(){
    qDebug()<<"Channels: Cannot open file"<<m_FileName;
    m_Channels->push_back("TVP+1");
    m_Channels->push_back("TVP+2");
}

void Channels::createFromFile(QFile &file){
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        m_Channels->push_back(line);
    }
}

void Channels::sort()
{
    if (!m_Channels->empty())
    {
    std::sort(m_Channels->begin(), m_Channels->end(),
          [](const QString &a, const QString &b){return a.toLower() < b.toLower();});
    }
}

void Channels::setFileName(QString &file){
    m_FileName = &file;
}

int Channels::size(){
    return m_Channels->size();
}

QString Channels::get(int i)
{
    return m_Channels->at(i);
}

QStringList Channels::getAll()
{
    return *m_Channels;
}

void Channels::pushBack(QString channel)
{
    m_Channels->push_back(channel);
}

void Channels::clear()
{
    m_Channels->clear();
}

void Channels::remove(int i)
{
    m_Channels->removeAt(i);
}

void Channels::remove(QString channel)
{
    int i = this->m_Channels->indexOf(channel);
    remove(i);
}

void Channels::setAll(QStringList channels)
{
    *m_Channels = channels;
}
