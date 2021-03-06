#ifndef CHANNELS_H
#define CHANNELS_H
class QFile;
class QString;
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
    void setAll (QStringList m_Channels);
private:
    QStringList *m_Channels;
    QString *m_FileName;
};

#endif // CHANNELS_H
