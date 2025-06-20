#include "character.h"
#include <QDebug>
#include <QGraphicsScene>

Character::Character(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/characters/goku.png").scaled(50, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Character::jump()
{
    if (!isJumping) {
        velocityY = -14;  // ← Aumenta el impulso del salto
        isJumping = true;
    }
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

    // Movimiento horizontal
    qreal newX = x() + velocityX;
    if (newX < 0) newX = 0;
    if (newX > scene()->width() - pixmap().width()) {
        newX = scene()->width() - pixmap().width();
    }

    // Movimiento vertical (salto parabólico)
    velocityY += gravity;
    qreal newY = y() + velocityY;

    if (newY >= groundLevel) {
        newY = groundLevel;
        velocityY = 0;
        isJumping = false;
    }

    setPos(newX, newY);
}
