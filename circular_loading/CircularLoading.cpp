#include <QPainter>
#include "CircularLoading.h"
#include <QDateTime>

namespace datall
{
namespace bases
{
CircularLoading::CircularLoading(QWidget *parent):
    QWidget(parent)
{
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // setWindowFlags(Qt::FramelessWindowHint);
    startTimer(100);
    setFixedSize(100, 100);
    // setMinimum(0);
    // setMaximum(360);
}

void  CircularLoading::paintEvent(QPaintEvent *)
{
    QPainter  painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::gray, 5, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(rect().adjusted(5, 5, -5, -5), -m_value * 16, 180 * 16);
}

void  CircularLoading::timerEvent(QTimerEvent *event)
{
    static qint64  counter = 0;

    counter += 10;
    m_value  = counter;/*QDateTime::currentDateTime().toMSecsSinceEpoch() % 3600*/
    update();
}

void  CircularLoading::showEvent(QShowEvent *event)
{
    move(qobject_cast<QWidget *>(parent())->rect().center() - rect().center());
}
} // namespace bases
} // namespace datall
