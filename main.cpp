#include "downloadmanagerwidget.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DownloadManagerWidget w;
    w.show();
    return a.exec();
}
