#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void realtimeDataPlot_1(QCustomPlot *customPlot);
    void realtimeDataPlot_2(QCustomPlot *customPlot);
    void realtimeDataPlot_3(QCustomPlot *customPlot);
    void realtimeDataPlot_4(QCustomPlot *customPlot);
    ~MainWindow();

private slots:
    void realtimeDataSlot_1();
    void realtimeDataSlot_2();
    void realtimeDataSlot_3();
    void realtimeDataSlot_4();


private:
    Ui::MainWindow *ui;
    QTimer dataTimer_1;
    QTimer dataTimer_2;
    QTimer dataTimer_3;
    QTimer dataTimer_4;
};
#endif // MAINWINDOW_H
