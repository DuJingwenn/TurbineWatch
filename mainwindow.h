#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void realtimeDataPlot(QCustomPlot *customPlot);
    ~MainWindow();

private slots:
    void realtimeDataSlot();


private:
    Ui::MainWindow *ui;
    QTimer plotDataTimer;
    int yLowAxisRange = -10;
    int yHighAxisRange = 10;
};
#endif // MAINWINDOW_H
