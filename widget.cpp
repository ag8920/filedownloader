#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStandardPaths>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->downloadProgressBar->setStyleSheet(QString::fromUtf8("text-align:center;"));
    m_downloader=new Downloader;

    connect(ui->downloadButton, &QPushButton::clicked,
            this, &Widget::slotDownloadButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &Widget::slotCancelButtonCliecked);
    connect(ui->selectButton, &QPushButton::clicked,
            this, &Widget::slotSelectTargetFolder);
    connect(m_downloader, &Downloader::signalUpdateDownloadProgress,
            this, &Widget::slotUpdateProgress);
}

Widget::~Widget()
{
    delete ui;
    delete  m_downloader;
}

void Widget::slotDownloadButtonClicked()
{

    m_downloader->get(ui->TargetFolderLineEdit->text(),ui->UrlLineEdit->text());
}

void Widget::slotCancelButtonCliecked()
{
    //отмена загрузки
    m_downloader->slotCancelDownload();
    ui->downloadProgressBar->setMaximum(100);
    ui->downloadProgressBar->setValue(0);
}

void Widget::slotSelectTargetFolder()
{
    QString targetFolder=QFileDialog::getExistingDirectory(this,
                                                           tr("Select folder"),
                                                           QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->TargetFolderLineEdit->setText(targetFolder);
}

void Widget::slotUpdateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->downloadProgressBar->setMaximum(bytesTotal);
    ui->downloadProgressBar->setValue(bytesReceived);
}
