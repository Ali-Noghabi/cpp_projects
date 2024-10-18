#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QValueAxis>
#include <QSettings>
#include "sensorwidget.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::on_actionAddSensor_triggered()
{
    auto  sw = new SensorWidget();

    ui->tabWidget->addTab(sw, QString("Sensor_%1").arg(ui->tabWidget->count()));
    ui->tabWidget->setCurrentWidget(sw);
    mSensorWidgets.append(sw);
}

void  MainWindow::on_actionSave_triggered()
{
    QByteArrayList  sensorSettings;

    for (auto i = 0; i < ui->tabWidget->count(); i++)
    {
        auto  sw = qobject_cast<SensorWidget *>(ui->tabWidget->widget(i));

        if (sw)
        {
            QByteArray   mapData;
            QDataStream  outStream(&mapData, QIODevice::WriteOnly);
            auto         map = sw->save();

            outStream << map;
            sensorSettings.append(mapData);
        }
    }

    QSettings  setting;

    setting.setValue("sensors", QVariant::fromValue(sensorSettings));

    setting.sync();
}

void  MainWindow::on_actionLoad_triggered()
{
    while (ui->tabWidget->widget(0))
    {
        ui->tabWidget->removeTab(0);
    }

    QSettings  setting;
    QVariant   sensors    = setting.value("sensors");
    auto       sensorList = sensors.value<QByteArrayList>();

    if (sensorList.isEmpty())
    {
        return;
    }

    for (auto s : sensorList)
    {
        QDataStream  inStream(&s, QIODevice::ReadOnly);
        QVariantMap  inMap;
        inStream >> inMap;

        auto  sw = new SensorWidget();

        ui->tabWidget->addTab(sw, QString("Sensor_%1").arg(ui->tabWidget->count()));
        mSensorWidgets.append(sw);

        sw->load(inMap);
    }
}

void  MainWindow::on_startStopAll_toggled(bool checked)
{
    if (checked)
    {
        ui->startStopAll->setText("Stop All");
    }
    else
    {
        ui->startStopAll->setText("Start All");
    }

    for (auto s : mSensorWidgets)
    {
        QMetaObject::invokeMethod(s, "triggerStartStopAll", Qt::QueuedConnection, Q_ARG(bool, checked));
    }
}

void  MainWindow::on_pbSetClients_clicked()
{
    for (auto s : mSensorWidgets)
    {
        QMetaObject::invokeMethod(s, "triggerSetHost", Qt::QueuedConnection, Q_ARG(const QHostAddress &, QHostAddress(ui->lineEdit->text())));
        QMetaObject::invokeMethod(s, "triggerSetPort", Qt::QueuedConnection, Q_ARG(int, ui->spinBox->value()));
    }
}
