#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsPixmapItem>
#include <QObject>

class Enemy: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Enemy(int firerate=3000, int type=0);
    bool goRight = true;
public slots:
    void move();
    void start_fire();
};

#endif // ENEMY_H
