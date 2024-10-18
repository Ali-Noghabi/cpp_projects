#ifndef DACHARTVIEW_H
#define DACHARTVIEW_H

#include <QChartView>

// QT_CHARTS_USE_NAMESPACE

class DaChartView: public QChartView
{
    Q_OBJECT

public:
    DaChartView(QChart *chartView);

    // QWidget interface

protected:
    void  wheelEvent(QWheelEvent *event) override;

    void  mouseReleaseEvent(QMouseEvent *event);
};

#endif // DACHARTVIEW_H
