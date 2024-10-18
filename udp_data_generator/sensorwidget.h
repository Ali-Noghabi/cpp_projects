#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QThread>
#include <QLineSeries>
#include <QScatterSeries>

#include "datagenerator.h"

// QT_CHARTS_USE_NAMESPACE

namespace Ui
{
class SensorWidget;
}

class ChartView;
class Chart;

class SensorWidget: public QWidget
{
    Q_OBJECT

public:
    explicit SensorWidget(QWidget *parent = nullptr);

    ~SensorWidget();

    QVariantMap  save();

    void         load(QVariantMap map);

public  slots:
    void         setAllHost(const QHostAddress &newHost);

    void         setAllPort(int newPort);

signals:
    void         triggerStartStopAll(bool checked);

    void         triggerSetHost(const QHostAddress &newHost);

    void         triggerSetPort(int newPort);

private slots:
    void         on_pb_setClient_clicked();

    void         on_pbInterval_clicked();

    void         on_pbStartStop_toggled(bool checked);

    void         on_pbSensorType_clicked();

    void         pointsChanged(QXYSeries *series);

    void         livePointsChanged(QList<QPointF> points);

    void         on_pbSetThreshold_clicked();

    void         on_pb_clicked();

    void         on_spinCUp_valueChanged(int arg1);

    void         on_spinWUp_valueChanged(int arg1);

    void         on_spinWDown_valueChanged(int arg1);

    void         on_spinCDown_valueChanged(int arg1);

    void         on_pushButton_clicked();

    void         on_scalarDataButton_clicked();

    void on_pbSetLength_clicked();

private:
    Ui::SensorWidget *ui;
    DataGenerator    *mDataGenerator   = nullptr;
    QThread          *mThreadGenerator = nullptr;
    ChartView        *mChartView       = nullptr;
    Chart            *mChart           = nullptr;
    QScatterSeries   *mScatter         = nullptr;
    QScatterSeries   *mScatter2        = nullptr;
    QLineSeries      *mLineSeries      = nullptr;
    QLineSeries      *mLiveData        = nullptr;
    QLineSeries      *mThr_w_up        = nullptr;
    QLineSeries      *mThr_w_down      = nullptr;
    QLineSeries      *mThr_c_up        = nullptr;
    QLineSeries      *mThr_c_down      = nullptr;
    QList<QPointF>    mC_up;
    QList<QPointF>    mC_down;
    QList<QPointF>    mW_up;
    QList<QPointF>    mW_down;
};

#endif // SENSORWIDGET_H
