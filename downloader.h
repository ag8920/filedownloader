#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DOWNLOADER_H