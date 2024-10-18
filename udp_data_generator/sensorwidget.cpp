#include "sensorwidget.h"
#include "ui_sensorwidget.h"

#include "chart/chart.h"
#include "chart/chartview.h"

#include <QValueAxis>

#define DATA_LENGTH     3600

SensorWidget::SensorWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SensorWidget)
{
    ui->setupUi(this);

    mThr_w_up   = new QLineSeries();
    mThr_w_down = new QLineSeries();
    mThr_c_up   = new QLineSeries();
    mThr_c_down = new QLineSeries();

    mLineSeries = new QLineSeries();
    mLiveData   = new QLineSeries();
    mScatter    = new QScatterSeries();
    mScatter2   = new QScatterSeries();

    mThr_w_up->setObjectName("thr_w_up");
    mThr_w_down->setObjectName("thr_w_down");
    mThr_c_up->setObjectName("thr_c_up");
    mThr_c_down->setObjectName("thr_c_down");
    mLineSeries->setObjectName("series");
    mScatter->setObjectName("scatter");
    mScatter2->setObjectName("scatter2");


    QList<QPointF>  points;

    for (int i = 0; i <= DATA_LENGTH; i += 100)
    {
        mC_up.append(QPointF(i, 900));
        mC_down.append(QPointF(i, 100));
        mW_up.append(QPointF(i, 800));
        mW_down.append(QPointF(i, 200));

        mLineSeries->append(i, 500);
        mScatter->append(i, 500);
        mScatter2->append(i, 100);
        mThr_w_up->append(i, 800);
        mThr_w_down->append(i, 200);
        mThr_c_up->append(i, 900);
        mThr_c_down->append(i, 100);

        points.append(QPointF(i, 500));
    }

// *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    mChart = new Chart(0, Qt::Window, mScatter);

    mChart->legend()->hide();
    mChart->addSeries(mThr_w_up);
    mChart->addSeries(mThr_w_down);
    mChart->addSeries(mThr_c_up);
    mChart->addSeries(mThr_c_down);
    mChart->addSeries(mLiveData);
    mChart->addSeries(mLineSeries);
    mChart->addSeries(mScatter);
    mChart->addSeries(mScatter2);

    mScatter->setMarkerSize(15);
    QPen  p = mLineSeries->pen();

    p.setWidth(2);
    mLineSeries->setPen(p);

// mChart->setTitle("Drag'n drop to move data points");

    QValueAxis *axisX = new QValueAxis();

    mChart->addAxis(axisX, Qt::AlignBottom);
    axisX->setRange(0, 3600);
    mLineSeries->attachAxis(axisX);

    mThr_w_up->attachAxis(axisX);
    mThr_w_down->attachAxis(axisX);
    mThr_c_up->attachAxis(axisX);
    mThr_c_down->attachAxis(axisX);
    mScatter->attachAxis(axisX);
    mScatter2->attachAxis(axisX);
    mLiveData->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();

    mChart->addAxis(axisY, Qt::AlignLeft);
    axisY->setRange(0, 1000);
    mLineSeries->attachAxis(axisY);

    mThr_w_up->attachAxis(axisY);
    mThr_w_down->attachAxis(axisY);
    mThr_c_up->attachAxis(axisY);
    mThr_c_down->attachAxis(axisY);
    mScatter->attachAxis(axisY);
    mScatter2->attachAxis(axisY);
    mLiveData->attachAxis(axisY);

    QObject::connect(mThr_w_up, &QLineSeries::pressed, mChart, &Chart::clickPoint);
    QObject::connect(mThr_w_down, &QLineSeries::pressed, mChart, &Chart::clickPoint);
    QObject::connect(mThr_c_up, &QLineSeries::pressed, mChart, &Chart::clickPoint);
    QObject::connect(mThr_c_down, &QLineSeries::pressed, mChart, &Chart::clickPoint);
    QObject::connect(mScatter, &QScatterSeries::pressed, mChart, &Chart::clickPoint);
    QObject::connect(mScatter2, &QScatterSeries::pressed, mChart, &Chart::clickPoint);

    QObject::connect(mChart, &Chart::pointsChange, this, &SensorWidget::pointsChanged);

    mChartView = new ChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);


    ui->chartLayout->addWidget(mChartView);

    mDataGenerator = new DataGenerator;


    mThreadGenerator = new QThread();
    mDataGenerator->moveToThread(mThreadGenerator);

    mDataGenerator->setPoints(points);

    mThreadGenerator->start();
    QMetaObject::invokeMethod(mDataGenerator, "initialize", Qt::QueuedConnection);

    connect(mDataGenerator, &DataGenerator::dataReady, this, &SensorWidget::livePointsChanged, Qt::QueuedConnection);

    connect(this, &SensorWidget::triggerStartStopAll, this, &SensorWidget::on_pbStartStop_toggled, Qt::QueuedConnection);

    connect(this, &SensorWidget::triggerSetHost, this, &SensorWidget::setAllHost, Qt::QueuedConnection);

    connect(this, &SensorWidget::triggerSetPort, this, &SensorWidget::setAllPort, Qt::QueuedConnection);
}

SensorWidget::~SensorWidget()
{
    QMetaObject::invokeMethod(mDataGenerator, "setStartStop", Qt::QueuedConnection, Q_ARG(bool, false));

    mThreadGenerator->quit();
    mThreadGenerator->wait();

    delete mThreadGenerator;


    delete ui;
}

QVariantMap  SensorWidget::save()
{
    QVariantMap  map;

// map["points"]     = QVariant::fromValue(mScatter->points());
// map["thr_w_up"]   = QVariant::fromValue(mThr_w_up->points());
// map["thr_w_down"] = QVariant::fromValue(mThr_w_down->points());
// map["thr_c_up"]   = QVariant::fromValue(mThr_c_up->points());
// map["thr_c_down"] = QVariant::fromValue(mThr_c_down->points());
// QByteArrayList  sensorSettings;
    QByteArray   chartData;
    QDataStream  outStream(&chartData, QIODevice::WriteOnly);

    outStream << mScatter->points();
    outStream << mThr_w_up->points();
    outStream << mThr_w_down->points();
    outStream << mThr_c_up->points();
    outStream << mThr_c_down->points();

    map["charts"]         = chartData;
    map["hardwareId"]     = QVariant::fromValue(ui->spinHardwareId->value());
    map["type"]           = QVariant::fromValue(ui->comboBox->currentIndex());
    map["address"]        = QVariant::fromValue(ui->linehost->text());
    map["port"]           = QVariant::fromValue(ui->spinPort->value());
    map["randomInterval"] = QVariant::fromValue(ui->spinMS->value());
    map["timeInterval"]   = QVariant::fromValue(ui->spinRandomInterval->value());

    map["thrCUp"]   = QVariant::fromValue(ui->spinCUp->value());
    map["thrCDown"] = QVariant::fromValue(ui->spinCDown->value());
    map["thrWUp"]   = QVariant::fromValue(ui->spinWUp->value());
    map["thrWDown"] = QVariant::fromValue(ui->spinWDown->value());

    return map;
}

void  SensorWidget::load(QVariantMap map)
{
    if (map.isEmpty())
    {
        return;
    }

    QByteArrayList  sensorSettings;
    auto            chartByteDatas = map["charts"].toByteArray();
    QDataStream     outStream(&chartByteDatas, QIODevice::ReadOnly);
    QList<QPointF>  chartData;

    outStream >> chartData;
    mScatter->replace(chartData);

    outStream >> chartData;
    mW_up = chartData;
    mThr_w_up->replace(chartData);
    outStream >> chartData;
    mW_down = chartData;
    mThr_w_down->replace(chartData);
    outStream >> chartData;
    mC_up = chartData;
    mThr_c_up->replace(chartData);
    outStream >> chartData;
    mC_down = chartData;
    mThr_c_down->replace(chartData);

    mLineSeries->replace(mScatter->points());

    ui->spinHardwareId->setValue(map["hardwareId"].toUInt());
    ui->comboBox->setCurrentIndex(map["type"].toInt());
    ui->linehost->setText(map["address"].toString());
    ui->spinPort->setValue(map["port"].toUInt());
    ui->spinMS->setValue(map["randomInterval"].toUInt());
    ui->spinRandomInterval->setValue(map["timeInterval"].toUInt());

    ui->spinCUp->setValue(map["thrCUp"].toInt());
    ui->spinCDown->setValue(map["thrCDown"].toInt());
    ui->spinWUp->setValue(map["thrWUp"].toInt());
    ui->spinWDown->setValue(map["thrWDown"].toInt());

    on_pbSensorType_clicked();
    on_pb_setClient_clicked();
    on_pbInterval_clicked();
}

void  SensorWidget::setAllHost(const QHostAddress &newHost)
{
    ui->linehost->setText(newHost.toString());
    mDataGenerator->setHost(QHostAddress(newHost));
}

void  SensorWidget::setAllPort(int newPort)
{
    ui->spinPort->setValue(newPort);
    mDataGenerator->setPort(newPort);
}

void  SensorWidget::on_pb_setClient_clicked()
{
    mDataGenerator->setHost(QHostAddress(ui->linehost->text()));
    mDataGenerator->setPort(ui->spinPort->value());
}

void  SensorWidget::on_pbInterval_clicked()
{
    auto  ms = ui->spinMS->value();

    QMetaObject::invokeMethod(mDataGenerator, "setInterval", Qt::QueuedConnection, Q_ARG(int, ms));
}

void  SensorWidget::on_pbStartStop_toggled(bool checked)
{
    if (checked)
    {
        ui->pbStartStop->setText("Stop");
    }
    else
    {
        ui->pbStartStop->setText("Start");
    }

    QMetaObject::invokeMethod(mDataGenerator, "setStartStop", Qt::QueuedConnection, Q_ARG(bool, checked));
}

void  SensorWidget::on_pbSensorType_clicked()
{
    mDataGenerator->setHardwareId(ui->spinHardwareId->value(), ui->comboBox->currentIndex());
}

void  SensorWidget::pointsChanged(QXYSeries *series)
{
    if (series->objectName() == "scatter")
    {
        auto        points = series->points();
        QList<int>  res;

        mLineSeries->replace(points);

        mDataGenerator->setPoints(points);
    }
}

void  SensorWidget::livePointsChanged(QList<QPointF> points)
{
    mLiveData->replace(points);
}

void  SensorWidget::on_pbSetThreshold_clicked()
{
    mThr_c_up->replace(mC_up);
    mThr_c_down->replace(mC_down);
    mThr_w_up->replace(mW_up);
    mThr_w_down->replace(mW_down);

    auto  axisY = mChart->axisY();

    axisY->setRange(ui->spinCDown->value(), ui->spinCUp->value() + 10);
}

void  SensorWidget::on_pb_clicked()
{
    mDataGenerator->setRandomInterval(ui->spinRandomInterval->value());
}

void  SensorWidget::on_spinCUp_valueChanged(int arg1)
{
    mC_up.clear();

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        mC_up.append(QPointF(i, arg1));
    }
}

void  SensorWidget::on_spinWUp_valueChanged(int arg1)
{
    mW_up.clear();

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        mW_up.append(QPointF(i, arg1));
    }
}

void  SensorWidget::on_spinWDown_valueChanged(int arg1)
{
    mW_down.clear();

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        mW_down.append(QPointF(i, arg1));
    }
}

void  SensorWidget::on_spinCDown_valueChanged(int arg1)
{
    mC_down.clear();

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        mC_down.append(QPointF(i, arg1));
    }
}

void  SensorWidget::on_pushButton_clicked()
{
    mDataGenerator->setBoardStatus(ui->comboBox_2->currentText().toInt());
}

void  SensorWidget::on_scalarDataButton_clicked()
{
    mDataGenerator->setScalarData(ui->scalarDataSpinBox->value());
}

void SensorWidget::on_pbSetLength_clicked()
{
    mDataGenerator->setDataLength(ui->spinDataSize->value());
}

