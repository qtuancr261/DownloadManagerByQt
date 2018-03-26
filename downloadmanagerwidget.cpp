#include "downloadmanagerwidget.h"
#include "ui_downloadmanagerwidget.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) :
    QWidget(parent),
    networkManager{new QNetworkAccessManager(parent)},
    ui(new Ui::DownloadManagerWidget),
    networkMapper{new QSignalMapper(this)},
    contentID{}
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    ui->groupBoxAuthenticationServer->setVisible(false);
    QObject::connect(ui->buttonDownload, &QPushButton::pressed, this, &DownloadManagerWidget::startDownload);
    //QObject::connect(networkManager, &QNetworkAccessManager::finished, this, &DownloadManagerWidget::downloadFinished);
    QObject::connect(networkMapper, SIGNAL(mapped(int)), this, SLOT(downloadFinished(int)));
    QObject::connect(networkManager, &QNetworkAccessManager::authenticationRequired, this, &DownloadManagerWidget::provideAuthentication);
}

void DownloadManagerWidget::downloadByHTTP()
{
    qDebug() << "HTTP Request";
    QNetworkReply* reply{networkManager->get(QNetworkRequest(QUrl(ui->lineEditURL->text())))};
    // Let map the reply
    QObject::connect(reply, SIGNAL(finished()), networkMapper, SLOT(map()));
    // store reply with a related ID in hash table
    repliesHash.insert(++contentID, reply);
    networkMapper->setMapping(reply, contentID);
}

void DownloadManagerWidget::downloadByFTP()
{
    qDebug() << "FTP Request";
    QUrl FTPReuqest{ui->lineEditURL->text()};
    //FTPReuqest.setUserName(QString("thieuquangtuan"));
    //FTPReuqest.setPassword(QString("thematrix141"));
    qDebug() << FTPReuqest.path().mid(1);
    FTPReuqest.setPath(FTPReuqest.path().mid(1));
    networkManager->get(QNetworkRequest(FTPReuqest));
}

void DownloadManagerWidget::provideAuthentication(QNetworkReply *replyFromServer, QAuthenticator *authenticator)
{
    if (ui->lineEditUsername->text().isEmpty() || ui->lineEditPassword->text().isEmpty())
    {
        //QMessageBox::information(this, "Server requires authentication", "Please provide server authentication to download this content");
        qDebug() << "Server requires authentication ";
        ui->buttonAuthenticationServer->setChecked(true);
        ui->lineEditUsername->setFocus();
        return;
    }
    authenticator->setUser(ui->lineEditUsername->text());
    authenticator->setPassword(ui->lineEditPassword->text());
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
        qDebug() << replyFromServer->request().url();
        downloadedFile.open(QIODevice::WriteOnly);
        //QDataStream fileWriter{&downloadedFile};
        //fileWriter << replyFromServer->readAll();
        downloadedFile.write(replyFromServer->readAll());
        replyFromServer->deleteLater();
    }
}

void DownloadManagerWidget::downloadFinished(int replyID)
{
    QNetworkReply* reply{repliesHash.take(replyID)};
    downloadFinished(reply);
}
