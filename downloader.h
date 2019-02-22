#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QSslError>
class QNetworkAccessManager;
class QNetworkReply;
class QFile;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    //запуск скачивания
    bool get(const QString &targetFolder, const QString &urlstring);
public slots:
    //отмена загрузки
    void slotCancelDownload();
signals:
    //информация о прогрессе загрузки
    void signalUpdateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private slots:
    //слот считывания загружаемых данных
    void slotReadyRead();
    // слот для обработки завершения запроса
    void slotReply(QNetworkReply *reply);

    void slotSslErrors(QNetworkReply *reply);
private:
    QNetworkReply *m_currentReply{nullptr}; //текущий обрабатываемый запрос
    QFile *m_file {nullptr}; //текущий файл в который идет запись
    QNetworkAccessManager *m_manager; //сетевой менеджер загрузки файлов

};

#endif // DOWNLOADER_H
