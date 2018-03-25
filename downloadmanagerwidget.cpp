#include "downloadmanagerwidget.h"
#include "ui_downloadmanagerwidget.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) :
    QWidget(parent),
    networkManager{new QNetworkAccessManager(parent)},
    ui(new Ui::DownloadManagerWidget)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    ui->groupBoxAuthenticationServer->setVisible(false);
    QObject::connect(ui->buttonDownload, &QPushButton::pressed, this, &DownloadManagerWidget::startDownload);
    QObject::connect(networkManager, &QNetworkAccessManager::finished, this, &DownloadManagerWidget::downloadFinished);
    QObject::connect(networkManager, &QNetworkAccessManager::authenticationRequired, this, &DownloadManagerWidget::provideAuthentication);
}

void DownloadManagerWidget::downloadByHTTP()
{
    qDebug() << "HTTP Request";
    networkManager->get(QNetworkRequest(QUrl(ui->lineEditURL->text())));
}

void DownloadManagerWidget::downloadByFTP()
{
    qDebug() << "FTP Request";
    QUrl FTPReuqest{ui->lineEditURL->text()};
    //FTPReuqest.setUserName(QString("thieuquangtuan"));
    //FTPReuqest.setPassword(QString("thematrix141"));
    FTPReuqest.setPath(FTPReuqest.fileName());
    networkManager->get(QNetworkRequest(FTPReuqest));
}

void DownloadManagerWidget::provideAuthentication(QNetworkReply *replyFromServer, QAuthenticator *authenticator)
{
    authenticator->setUser("thieuquangtuan");
    authenticator->setPassword("thematrix141");
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
        {
            downloadByHTTP();
        }
        else if (downloadContent.scheme() == "ftp")
        {
            downloadByFTP();
        }

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
        QFile downloadedFile{QUrl(ui->lineEditURL->text()).fileName()};
        downloadedFile.open(QIODevice::WriteOnly);
        //QDataStream fileWriter{&downloadedFile};
        //fileWriter << replyFromServer->readAll();
        downloadedFile.write(replyFromServer->readAll());
        replyFromServer->deleteLater();
    }
}
