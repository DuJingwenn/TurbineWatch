#include "mainwindow.h"

#include <QApplication>
#include <QtCharts/QLegend>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
