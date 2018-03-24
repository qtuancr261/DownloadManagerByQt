#include "downloadmanagerwidget.h"
#include "ui_downloadmanagerwidget.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) :
    QWidget(parent),
    networkManager{new QNetworkAccessManager(parent)},
    ui(new Ui::DownloadManagerWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonDownload, &QPushButton::pressed, this, &DownloadManagerWidget::startDownload);
    QObject::connect(networkManager, &QNetworkAccessManager::finished, this, &DownloadManagerWidget::downloadFinished);
}

void DownloadManagerWidget::downloadByHTTP()
{

}

void DownloadManagerWidget::downloadByFTP()
{

}

DownloadManagerWidget::~DownloadManagerWidget()
{
    delete ui;
}

void DownloadManagerWidget::startDownload()
{
    QUrl downloadContent{ui->lineEditURL->text()};
    if (downloadContent.isEmpty())
    {
        QMessageBox::information(this, "Invalid URL", "Please use a valid URL for downloading.", QMessageBox::Ok);
        ui->lineEditURL->setFocus();
    }
    else
    {
        if (downloadContent.scheme() == "http")
            qDebug() << "HTTP Request";
        else if (downloadContent.scheme() == "ftp")
            qDebug() << "FTP Request";
        networkManager->get(QNetworkRequest(QUrl(ui->lineEditURL->text())));
        QMessageBox::information(this, "File", downloadContent.fileName(), QMessageBox::Ok);
    }
}

void DownloadManagerWidget::downloadFinished(QNetworkReply *replyFromServer)
{
    if (replyFromServer->error())
    {
        QMessageBox::information(this, "Error when downloading", replyFromServer->errorString());
    }
    else
    {
        //ui->pTextEditPreview->setPlainText(replyFromServer->readAll());
        QFile downloadedFile{"SmartBoy.pdf"};
        downloadedFile.open(QIODevice::WriteOnly);
        //QDataStream fileWriter{&downloadedFile};
        //fileWriter << replyFromServer->readAll();
        downloadedFile.write(replyFromServer->readAll());
        replyFromServer->deleteLater();
    }
}
