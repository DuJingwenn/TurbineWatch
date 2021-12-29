#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TurbineWatch");
    statusBar()->clearMessage();
    ui->plot_1->replot();
    ui->plot_2->replot();
    ui->plot_3->replot();
    ui->plot_4->replot();
    realtimeDataPlot_1(ui->plot_1);
    realtimeDataPlot_2(ui->plot_2);
    realtimeDataPlot_3(ui->plot_3);
    realtimeDataPlot_4(ui->plot_4);
}

void MainWindow::realtimeDataPlot_1(QCustomPlot *customPlot)
{

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);

    customPlot->xAxis->setLabel("时间(Time/s)");
    customPlot->yAxis->setLabel("转速(RPM)");
    //customPlot->legend->setVisible(true);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer_1, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_1()));
    dataTimer_1.start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataPlot_2(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    customPlot->xAxis->setLabel("时间(Time/s)");
    customPlot->yAxis->setLabel("输出功率(KW)");
    //customPlot->legend->setVisible(true);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer_2, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_2()));
    dataTimer_2.start(10); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataPlot_3(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    customPlot->xAxis->setLabel("时间(Time/s)");
    customPlot->yAxis->setLabel("燃料质量流量(kg/s)");
    //customPlot->legend->setVisible(true);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer_3, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_3()));
    dataTimer_3.start(100); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataPlot_4(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    customPlot->xAxis->setLabel("时间(Time/s)");
    customPlot->yAxis->setLabel("控制系统流量输出(%)");
    //customPlot->legend->setVisible(true);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer_4, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_4()));
    dataTimer_4.start(300); // Interval 0 means to refresh as fast as possible

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::realtimeDataSlot_1()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plot_1->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->plot_1->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot_1->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot_1->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot_1->graph(0)->data()->size()+ui->plot_1->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::realtimeDataSlot_2()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plot_2->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->plot_2->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot_2->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot_2->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot_2->graph(0)->data()->size()+ui->plot_2->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }

}

void MainWindow::realtimeDataSlot_3()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plot_3->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->plot_3->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot_3->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot_3->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot_3->graph(0)->data()->size()+ui->plot_3->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }

}

void MainWindow::realtimeDataSlot_4()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plot_4->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->plot_4->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot_4->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plot_4->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot_4->graph(0)->data()->size()+ui->plot_4->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }

}

void MainWindow::on_pushButton_28_clicked()
{
    if(ui->pushButton_28->text()=="暂停"){
        dataTimer_1.stop();
        dataTimer_2.stop();
        dataTimer_3.stop();
        dataTimer_4.stop();
        ui->pushButton_28->setText("继续");
    }else{
        dataTimer_1.start();
        dataTimer_2.start();
        dataTimer_3.start();
        dataTimer_4.start();
        ui->pushButton_28->setText("暂停");
    }

}

