#include "enemy.h"

Enemy::Enemy(QGraphicsItem *parent) : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/piccolo.png").scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 700;
    setY(groundLevel - pixmap().height());

    // Inicializar salud
    maxHealth = 100;
    currentHealth = 100;
}
