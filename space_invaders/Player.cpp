#include "Player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include "Bullet.h"
#include "Enemy.h"


Player::Player(QGraphicsItem *parent) : QGraphicsPixmapItem(parent){
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/bulletsound.mp3"));
    setPixmap(QPixmap(":/images/player.png"));
    QTimer *flagtimer = new QTimer();
    connect(flagtimer, SIGNAL(timeout()), this, SLOT(set_flag()));
    flagtimer -> start(1000);
}
void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left){
        if(pos().x() > 0)
            setPos(x()-10, y());
    }
    else if(event->key() == Qt::Key_Right){
        if(pos().x() < 760)
            setPos(x()+10, y());
    }
    else if(event->key() == Qt::Key_Space && flag){
        flag = false;
        Bullet *bullet = new Bullet();
        bullet->setPos(x()+16, y());
        scene()->addItem(bullet);
        if(bulletsound->state()==QMediaPlayer::PlayingState){
            bulletsound->setPosition(0);
        }
        else if (bulletsound->state()==QMediaPlayer::StoppedState) {
            bulletsound->play();
        }
    }
}

void Player::set_flag()
{
    flag = true;
}
