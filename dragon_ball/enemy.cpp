#include "enemy.h"

Enemy::Enemy(QGraphicsItem *parent) : Character(parent)
{
    health = 50;
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/piccolo.png").scaled(80, 100));
}
