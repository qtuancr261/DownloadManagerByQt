#include "downloadmanagerwidget.h"
#include "ui_downloadmanagerwidget.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) :
    QWidget(parent),
    networkManager{new QNetworkAccessManager(parent)},
    ui(new Ui::DownloadManagerWidget),
    networkMapper{new QSignalMapper(this)},
    contentID{},
    savedLocation{}
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    ui->lineEditSaveLocation->setText(QDir::homePath());
    ui->groupBoxAuthenticationServer->setVisible(false);
    QObject::connect(ui->buttonDownload, &QPushButton::pressed, this, &DownloadManagerWidget::startDownload);
    //QObject::connect(networkManager, &QNetworkAccessManager::finished, this, &DownloadManagerWidget::downloadFinished);
    QObject::connect(networkMapper, SIGNAL(mapped(QString)), this, SLOT(downloadFinished(QString)));
    QObject::connect(networkManager, &QNetworkAccessManager::authenticationRequired, this, &DownloadManagerWidget::provideAuthentication);
    QObject::connect(ui->buttonChooseSavedLocation, &QPushButton::pressed, this, &DownloadManagerWidget::chooseSavedLocation);
}

void DownloadManagerWidget::downloadByHTTP()
{
    qDebug() << "HTTP Request";
    QUrl HTPPRequest{ui->lineEditURL->text()};
    QNetworkReply* reply{networkManager->get(QNetworkRequest(QUrl(ui->lineEditURL->text())))};
    // Let map the reply
    QObject::connect(reply, SIGNAL(finished()), networkMapper, SLOT(map()));
    QObject::connect(reply, &QNetworkReply::downloadProgress, this, &DownloadManagerWidget::indicateDownloadProgress);
    // store reply with a related ID in hash table
    QString replyKey{QString::number(++contentID) + "|" + HTPPRequest.fileName()};
    repliesHash.insert(replyKey, reply);
    networkMapper->setMapping(reply, replyKey);
}

void DownloadManagerWidget::downloadByFTP()
{
    qDebug() << "FTP Request";
    QUrl FTPRequest{ui->lineEditURL->text()};
    FTPRequest.setPath(FTPRequest.path().mid(1));
    QNetworkReply* reply{networkManager->get(QNetworkRequest(FTPRequest))};
    QObject::connect(reply, SIGNAL(finished()), networkMapper, SLOT(map()));
    QObject::connect(reply, &QNetworkReply::downloadProgress, this, &DownloadManagerWidget::indicateDownloadProgress);
    QString replyKey{QString::number(++contentID) + "|" + FTPRequest.fileName()};

    repliesHash.insert(replyKey, reply);
    networkMapper->setMapping(reply, replyKey);

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
        return;
    }
    savedLocation = ui->lineEditSaveLocation->text();
    if (!savedLocation.exists())
        QMessageBox::warning(this, "Couldn't find the location", QString("The location: %1 doesn't exists. Please check it again" ).arg(savedLocation.path()));
    else
    {
        // Disable the saved location line edit when start downloading
        ui->lineEditSaveLocation->setDisabled(true);
        if (downloadContent.scheme() == "http" || downloadContent.scheme() == "https")
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

void DownloadManagerWidget::chooseSavedLocation()
{
    savedLocation = QFileDialog::getExistingDirectory(this, "Select the saved location", QDir::currentPath());
    if (!savedLocation.isEmpty())
        ui->lineEditSaveLocation->setText(savedLocation.absolutePath());
}

void DownloadManagerWidget::downloadFinished(const QString& replyID)
{
    QNetworkReply* replyFromServer{repliesHash.take(replyID)};
    qDebug() << replyID.section('|', 1, 1);
    if (replyFromServer->error())
    {
        QMessageBox::information(this, "Error when downloading", replyFromServer->errorString());
    }
    else
    {
        //ui->pTextEditPreview->setPlainText(replyFromServer->readAll());
        QFile downloadedFile{savedLocation.absolutePath().append("/%1").arg(replyID.section('|', 1, 1))};
        qDebug() << replyFromServer->header(QNetworkRequest::ContentLengthHeader).toDouble() / (1024*1024)  << " mb";
        //qDebug() << replyFromServer->request().url();
        downloadedFile.open(QIODevice::WriteOnly);
        //QDataStream fileWriter{&downloadedFile};
        //fileWriter << replyFromServer->readAll();
        downloadedFile.write(replyFromServer->readAll());
        replyFromServer->deleteLater();
    }

    ui->lineEditSaveLocation->setEnabled(true);
}

void DownloadManagerWidget::indicateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qreal percent{bytesReceived < 1 ? 1.0 : static_cast<qreal>(bytesReceived) / bytesTotal};
    ui->labelDownloadedSize->setText(MeasurementUnit::getConvenientUnitFromBytes(bytesReceived) + "/" + MeasurementUnit::getConvenientUnitFromBytes(bytesTotal));
    ui->progressBarDowloadedSize->setValue(percent * ui->progressBarDowloadedSize->maximum());
}
