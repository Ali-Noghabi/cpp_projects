#include <Enemy.h>
#include <QTimer>
#include <QGraphicsScene>
#include <stdlib.h>
#include "Game.h"
#include "Fire.h"

Enemy::Enemy(int firerate, int type)
{
    int random_x = rand() % 700;
    int random_y = rand() % 200;
    setPos(random_x+50, random_y+50);
    if(type==0){
        setPixmap(QPixmap(":/images/s_green.png"));
        goRight = false;
    }
    else if(type==1){
        setPixmap(QPixmap(":/images/s_orange.png"));
        QTimer *firetimer = new QTimer();
        connect(firetimer, SIGNAL(timeout()), this, SLOT(start_fire()));
        firetimer -> start(firerate);
        goRight = true;
    }
    else{
        goRight = true;
        setPixmap(QPixmap(":/images/s_purple.png"));
    }
    QTimer *movetimer = new QTimer();
    connect(movetimer, SIGNAL(timeout()), this, SLOT(move()));
    movetimer -> start(30);
}

void Enemy::move()
{
    if(goRight){
        if(pos().x() > 750)
            goRight = false;
        setPos(x()+2, y());
    }
    else{
        if(pos().x() < 0)
            goRight = true;
        setPos(x()-2, y());
    }
}

void Enemy::start_fire()
{
    Fire *fire = new Fire();
    scene()->addItem(fire);
    fire->setPos(x()-5, y());
}
