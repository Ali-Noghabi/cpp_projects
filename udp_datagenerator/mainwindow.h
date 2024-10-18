#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QThread>

#include "dachartview.h"
#include "datagenerator.h"
// QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class SensorWidget;
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void  on_actionAddSensor_triggered();

    void  on_actionSave_triggered();

    void  on_actionLoad_triggered();

    void  on_startStopAll_toggled(bool checked);

    void  on_pbSetClients_clicked();

private:
    Ui::MainWindow        *ui;
    QList<SensorWidget *>  mSensorWidgets;
};

#endif // MAINWINDOW_H
