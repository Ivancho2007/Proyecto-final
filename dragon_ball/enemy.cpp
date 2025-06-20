#include "enemy.h"

Enemy::Enemy(QGraphicsItem *parent) : Character(parent)
{
    // Tamaño adecuado para Piccolo
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/piccolo.png").scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 700; // Mismo nivel del suelo que Goku
    setY(groundLevel - pixmap().height());
}
