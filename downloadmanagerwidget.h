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
#include <QtMath>
#include <QFileDialog>
#include "measurementunit.h"
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
    ~DownloadManagerWidget() override;

private:
    QNetworkAccessManager* networkManager;
    Ui::DownloadManagerWidget *ui;
    QHash<QString, QNetworkReply*> repliesHash;
    QSignalMapper* networkMapper;
    int contentID;
    QDir savedLocation;
    void startDownload(); // from URL
    void chooseSavedLocation();
private slots:
    void downloadFinished(const QString& replyID);
    void indicateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // DOWNLOADMANAGERWIDGET_H
