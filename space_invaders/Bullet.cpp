#include <Bullet.h>
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <Enemy.h>
#include <Fire.h>
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet()
{
    setPixmap(QPixmap(":/images/bullet.png"));
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer -> start(10);
}

extern Game *game;
void Bullet::move()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for(int i = 0, n = colliding_items.size(); i < n; i++){
        if(typeid(*(colliding_items[i]))==typeid (Enemy)){
            game->score->increase();
            game->enemy_count -= 1;
            if (game->enemy_count == 0){
                QGraphicsTextItem *text = new QGraphicsTextItem();
                text->setPlainText(QString("You Won :)"));
                text->setDefaultTextColor(Qt::darkBlue);
                text->setFont(QFont("times", 34));
                text->setPos(240, 250);
                scene()->addItem(text);
            }
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;
            return;
        }
        if(typeid(*(colliding_items[i]))==typeid (Fire)||typeid(*(colliding_items[i]))==typeid (Wall)){
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;
            return;
        }
    }
    setPos(x(), y()-5);
    if(pos().y() < 0){
        scene()->removeItem(this);
        delete this;
    }
}
