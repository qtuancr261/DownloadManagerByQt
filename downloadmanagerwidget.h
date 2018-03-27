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
#include <QHash>
#include <QSignalMapper>
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
    QHash<QString, QNetworkReply*> repliesHash;
    QSignalMapper* networkMapper;
    int contentID;
    void startDownload(); // from URL
private slots:
    void downloadFinished(QString replyID);
};

#endif // DOWNLOADMANAGERWIDGET_H
