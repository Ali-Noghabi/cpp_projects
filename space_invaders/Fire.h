#ifndef FIRE_H
#define FIRE_H
#include <QObject>
#include <QGraphicsPixmapItem>


#include <QTimer>
class Fire: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Fire();

public slots:
    void move();
};
#endif // FIRE_H
