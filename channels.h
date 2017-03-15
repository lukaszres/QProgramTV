#ifndef CHANNELS_H
#define CHANNELS_H
#include <QVector>
#include <QFile>

class Channels
{
public:
    Channels();

    void setFileAllChannels(QString &file);
    void createAndSortAllChannels();
    void createAllChannelsFromFile(QFile &file);
    void generateAllChannels();
    QString getChannelFromAllChannels(int i);
    int allChannelsSize();

    void setFileFavouritesChannels(QString &file);
    void createFavChannels();
    void createFavoritesChannelsFromFile(QFile &file);
    void sortFavChannels();
    int favouriteChannelsSize();
    void favouriteChannelsPushBack(QString channel);
    void favouriteChannelsClear();
    QString getChannellFromFavouriteChannels(int i);
    QStringList getFavouriteChannels();
    void removeFromFavouritesChannels(int i);

    void sortLeftChannels();
    int leftChannelsSize();
    void leftChannelsPushBack(QString channel);
    QStringList getLeftChannels();
    void removeFromLeftChannels(int i);
    void removeFromLeftChannels(QString channel);

    void setLeftChannelsToAll();

private:
    QString fileAllChannels;
    QVector<QString> allChannels;
    QString fileFavouritesChannels;
    QStringList favouriteChannels;
    QStringList leftChannels;
};

#endif // CHANNELS_H
