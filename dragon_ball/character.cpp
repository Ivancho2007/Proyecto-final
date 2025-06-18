#include "character.h"
#include <QDebug>
#include <QGraphicsScene>

Character::Character(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/characters/goku.png").scaled(50, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Character::jump()
{
    // Por ahora, no hace nada
}

void Character::moveLeft()
{
    velocityX = -moveSpeed;
    facingRight = false;
    isRunning = true;
}

void Character::moveRight()
{
    velocityX = moveSpeed;
    facingRight = true;
    isRunning = true;
}

void Character::stopMoving()
{
    velocityX = 0;
    isRunning = false;
}

void Character::basicAttack()
{
    qDebug() << "Basic attack performed";
}

void Character::advance(int phase)
{
    if (!phase) return;

    qreal newX = x() + velocityX;

    // Evita que salga de la escena
    if (newX < 0) newX = 0;
    if (newX > scene()->width() - pixmap().width()) {
        newX = scene()->width() - pixmap().width();
    }

    setX(newX);
}
