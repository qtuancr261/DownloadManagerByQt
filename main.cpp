#include "downloadmanagerwidget.h"
#include "measurementunit.h"
#include <QApplication>
// initialize static member
QVector<QString> MeasurementUnit::units{"B", "KB", "MB", "GB", "TB"};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DownloadManagerWidget w;
    qDebug() << MeasurementUnit::getConvenientUnitFromBytes(4096) << qPow(10, 10);
    w.show();
    return a.exec();
}
