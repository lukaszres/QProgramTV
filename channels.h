#ifndef CHANNELS_H
#define CHANNELS_H
#include <QVector>
#include <QFile>

class Channels
{
public:
    Channels();

    void createAndSortAllChannels();
    void generateAllChannels();
    void createAllChannelsFromFile(QFile &file);

    void createFavChannels();
    void createFavoritesChannelsFromFile(QFile &file);
    void sortFavChannels();
    void sortLeftChannels();

    void setFileAllChannels(QString &file);
    void setFileFavouritesChannels(QString &file);
    int allChannelsSize();

    int favouriteChannelsSize();
    void favouriteChannelsPushBack(QString channel);
    void favouriteChannelsClear();

    QString getChannelFromAllChannels(int i);
    QString getChannellFromFavouriteChannels(int i);
    QStringList getFavouriteChannels();
    QStringList getLeftChannels();
    void setLeftChannelsToAll();

    void leftChannelsPushBack(QString channel);
    void removeFromFavouritesChannels(int i);
    int leftChannelsSize();
    void removeFromLeftChannels(int i);
    void removeFromLeftChannels(QString channel);
private:
    QString fileAllChannels;
    QVector<QString> allChannels;
    QString fileFavouritesChannels;
    QStringList favouriteChannels;
    QStringList leftChannels;
};

#endif // CHANNELS_H
