#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //ui->groupBox_3->resize(ui->splitter_2->width(),0.2*ui->splitter_2->height());


    setWindowTitle("TurbineWatch");
    statusBar()->clearMessage();
    ui->customPlot->replot();
    realtimeDataPlot(ui->customPlot);
}

void MainWindow::realtimeDataPlot(QCustomPlot *customPlot)
{

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(yLowAxisRange, yHighAxisRange);

    customPlot->xAxis->setLabel("xlabel name");
    customPlot->yAxis->setLabel("ylabel name");
    customPlot->legend->setVisible(true);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&plotDataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    plotDataTimer.start(0); // Interval 0 means to refresh as fast as possible

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::realtimeDataSlot()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    ui->plainTextEdit->appendPlainText(QString::number(key, 10, 4));
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02) // at most add point every 20 ms
    {
      // add data to lines:
      double graph0Data = 20*qSin(key);
      double graph1Data = 20*qCos(key);
      ui->customPlot->graph(0)->addData(key, graph0Data);//+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->customPlot->graph(1)->addData(key, graph1Data);//+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      if(graph0Data >= yHighAxisRange || graph1Data >= yHighAxisRange){
          yHighAxisRange = (graph0Data > graph1Data) ? graph0Data : graph1Data;
          yHighAxisRange += 0.2 * yHighAxisRange;
          ui->customPlot->yAxis->setRange(yLowAxisRange, yHighAxisRange);
          ui->customPlot->replot();
      }
      if(graph0Data <= yLowAxisRange || graph1Data <= yLowAxisRange){
          yLowAxisRange = (graph0Data > graph1Data) ? graph1Data : graph0Data;
          yLowAxisRange += 0.2 * yLowAxisRange;
          ui->customPlot->yAxis->setRange(yLowAxisRange, yHighAxisRange);
          ui->customPlot->replot();
      }
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis(true);
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignLeft);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}


