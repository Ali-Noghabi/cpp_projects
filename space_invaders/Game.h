#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Score.h"
#include "Health.h"

class Game:public QGraphicsView{
public:
    Game(QWidget *parent=0);
    QGraphicsScene *scene;
    Score *score;
    Health *health;
    int enemy_count;
};

#endif // GAME_H
