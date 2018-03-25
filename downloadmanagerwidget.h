#ifndef DOWNLOADMANAGERWIDGET_H
#define DOWNLOADMANAGERWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QAuthenticator>
namespace Ui {
class DownloadManagerWidget;
}

class DownloadManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadManagerWidget(QWidget *parent = nullptr);
    void downloadByHTTP();
    void downloadByFTP();
    void provideAuthentication(QNetworkReply* replyFromServer, QAuthenticator* authenticator);
    ~DownloadManagerWidget();

private:
    QNetworkAccessManager* networkManager;
    Ui::DownloadManagerWidget *ui;
    void startDownload(); // from URL
    void downloadFinished(QNetworkReply* replyFromServer);
};

#endif // DOWNLOADMANAGERWIDGET_H
