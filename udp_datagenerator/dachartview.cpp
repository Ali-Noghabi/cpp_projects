#include "dachartview.h"

DaChartView::DaChartView(QChart *chartView):
    QChartView(chartView)
{
}

void  DaChartView::wheelEvent(QWheelEvent *event)
{
    qreal  factor = event->angleDelta().y() > 0 ? 0.5 : 2.0;

    chart()->zoom(factor);
    event->accept();
    QChartView::wheelEvent(event);
}

void  DaChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        chart()->zoomReset();
    }

    QChartView::mouseReleaseEvent(event);
}
