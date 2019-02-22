#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "downloader.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void slotDownloadButtonClicked();
    void slotCancelButtonCliecked();
    void slotSelectTargetFolder();
    void slotUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    Ui::Widget *ui;
    Downloader *m_downloader;
};

#endif // WIDGET_H
