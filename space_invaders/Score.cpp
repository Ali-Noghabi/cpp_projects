#include <Score.h>
#include <QFont>

Score::Score(QGraphicsItem *parent):QGraphicsTextItem(parent)
{
    score = 0;
    setPlainText(QString("score: ")+QString::number(score));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("times", 16));
}

void Score::increase()
{
    score++;
    setPlainText(QString("score: ")+QString::number(score));
}

int Score::getScore()
{
    return score;
}
