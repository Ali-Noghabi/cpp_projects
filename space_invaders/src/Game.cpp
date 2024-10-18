#include <Game.h>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <Enemy.h>
#include <Wall.h>
#include <QMediaPlayer>
#include <QImage>


Game::Game(QWidget *parent)
{
    enemy_count = 8;
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    scene->setBackgroundBrush(QBrush(Qt::black));
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);
    view->show();
    Player *player = new Player();
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPos(view->width()/2, view->height()-80);
    Enemy *enemy1 = new Enemy();
    scene->addItem(enemy1);
    Enemy *enemy2 = new Enemy();
    scene->addItem(enemy2);
    Enemy *enemy3 = new Enemy();
    scene->addItem(enemy3);
    Enemy *enemy4 = new Enemy(3000, 1);
    scene->addItem(enemy4);
    Enemy *enemy5 = new Enemy(4000, 1);
    scene->addItem(enemy5);
    Enemy *enemy6 = new Enemy(2000, 1);
    scene->addItem(enemy6);
    Enemy *enemy7 = new Enemy(0, 3);
    scene->addItem(enemy7);
    Enemy *enemy8 = new Enemy(0, 3);
    scene->addItem(enemy8);
    for(int x=10, i=0 ; i<4 ; i++){
        Wall *wall1 = new Wall(x+35, 400);
        scene->addItem(wall1);
        Wall *wall2 = new Wall(x+50, 400);
        scene->addItem(wall2);
        Wall *wall3 = new Wall(x+65, 400);
        scene->addItem(wall3);
        Wall *wall4 = new Wall(x+80, 400);
        scene->addItem(wall4);
        Wall *wall5 = new Wall(x+95, 400);
        scene->addItem(wall5);
        Wall *wall6 = new Wall(x+110, 400);
        scene->addItem(wall6);
        Wall *wall7 = new Wall(x+125, 400);
        scene->addItem(wall7);
        Wall *wall8 = new Wall(x+140, 400);
        scene->addItem(wall8);
        Wall *wall9 = new Wall(x+35, 385);
        scene->addItem(wall9);
        Wall *wall10 = new Wall(x+50, 385);
        scene->addItem(wall10);
        Wall *wall11 = new Wall(x+65, 385);
        scene->addItem(wall11);
        Wall *wall12 = new Wall(x+80, 385);
        scene->addItem(wall12);
        Wall *wall13 = new Wall(x+95, 385);
        scene->addItem(wall13);
        Wall *wall14 = new Wall(x+110, 385);
        scene->addItem(wall14);
        Wall *wall15 = new Wall(x+125, 385);
        scene->addItem(wall15);
        Wall *wall16 = new Wall(x+140, 385);
        scene->addItem(wall16);
        Wall *wall17 = new Wall(x+35, 370);
        scene->addItem(wall17);
        Wall *wall18 = new Wall(x+65, 370);
        scene->addItem(wall18);
        Wall *wall19 = new Wall(x+110, 370);
        scene->addItem(wall19);
        Wall *wall20 = new Wall(x+140, 370);
        scene->addItem(wall20);
        x += 200;
    }
    score = new Score();
    health = new Health();
    health->setPos(health->x(), health->y()+40);
    scene->addItem(score);
    scene->addItem(health);
    QMediaPlayer *music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/bgsound.mp3"));
    music->play();
}
