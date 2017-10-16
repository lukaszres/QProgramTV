#ifndef CHANNELS_H
#define CHANNELS_H
#include <QVector>
#include <QFile>
#include <QStringList>
class QStringList;

class Channels
{
public:
    Channels();

    void setFileName(QString &file);
    void create();
    void createFromFile(QFile &file);
    void generate();
    void sort();
    QString get(int i);
    int size();
    void pushBack(QString channel);
    void clear();
    QStringList getAll();
    void remove(int i);
    void remove(QString channel);
    void setAll (QStringList channels);

private:
    QStringList channels{};
    QString fileName;
};

#endif // CHANNELS_H
