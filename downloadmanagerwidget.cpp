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

DownloadManagerWidget::~DownloadManagerWidget()
{
    delete ui;
}

void DownloadManagerWidget::startDownload()
{
    QString downloadContent{ui->lineEditURL->text()};
    if (downloadContent.isEmpty())
    {
        QMessageBox::information(this, "Invalid URL", "Please use a valid URL for downloading.", QMessageBox::Ok);
        ui->lineEditURL->setFocus();
    }
    networkManager->get(QNetworkRequest(QUrl(ui->lineEditURL->text())));
}

void DownloadManagerWidget::downloadFinished(QNetworkReply *replyFromServer)
{
    if (replyFromServer->error())
    {
        ui->pTextEditPreview->setPlainText(replyFromServer->errorString());
    }
    else
    {
        ui->pTextEditPreview->setPlainText(replyFromServer->readAll());
        replyFromServer->deleteLater();
    }
}
