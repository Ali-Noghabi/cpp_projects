#include <Health.h>
#include <QFont>


Health::Health(QGraphicsItem *parent):QGraphicsTextItem(parent)
{
    health = 3;
    setPlainText(QString("health: ")+QString::number(health));
    setDefaultTextColor(Qt::yellow);
    setFont(QFont("times", 16));
}

void Health::decrease()
{
    health--;
    setPlainText(QString("health: ")+QString::number(health));
}

int Health::getHealth()
{
    return health;
}

