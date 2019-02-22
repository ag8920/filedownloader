#include "downloader.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

Downloader::Downloader(QObject *parent) : QObject(parent)
{
    m_manager=new QNetworkAccessManager(this);
    connect(m_manager,&QNetworkAccessManager::finished,
            this,&Downloader::slotReply);
    connect(m_manager, &QNetworkAccessManager::finished,
            m_manager, &QNetworkAccessManager::deleteLater);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &Downloader::slotSslErrors);
}

bool Downloader::get(const QString &targetFolder, const QString &urlstring)
{
    QUrl url=urlstring;
    if(targetFolder.isEmpty() || url.isEmpty())
        return false;
    //создаем объект класса файла для скачивания
    m_file=new QFile(targetFolder + QDir::separator()+url.fileName());
    //проверяем создан ли файл
    if(!m_file->open(QIODevice::WriteOnly))
    {
        delete m_file;
        m_file=nullptr;
        return false;
    }
    //создаем запрос
    QNetworkRequest request(url);
    //разрешаем переходить по редиректам
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    //запускаем скачивание
    m_currentReply=m_manager->get(request);

    //подключаемся к сигналам о готовности данных к чтению и обновлению
    //прогресса  скачивания
    connect(m_currentReply, &QNetworkReply::readyRead,
            this, &Downloader::slotReadyRead);
    connect(m_currentReply, &QNetworkReply::downloadProgress,
            this, &Downloader::signalUpdateDownloadProgress);
    return true;
}

void Downloader::slotCancelDownload()
{
    //отмена запроса
    if(m_currentReply)
        m_currentReply->abort();
}

void Downloader::slotReadyRead()
{
    //если есть данные и файл открыт
    if(m_file)
        //записываем их в файл
        m_file->write(m_currentReply->readAll());
}

void Downloader::slotReply(QNetworkReply *reply)
{
    //по завершению запрооса
    if(reply->error()==QNetworkReply::NoError)
    {
        //сохраняем файл
        m_file->flush();
        m_file->close();
    }
    else {
        //или удаляем его в случае ошибки
        m_file->remove();
    }
    delete  m_file;
    m_file=nullptr;
    reply->deleteLater();
}

void Downloader::slotSslErrors(QNetworkReply *reply)
{
    reply->ignoreSslErrors();
}
