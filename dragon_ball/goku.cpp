#include "goku.h"

Goku::Goku(QGraphicsItem *parent) : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/goku.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 500;
    setY(groundLevel - pixmap().height()+15);

}

void Goku::jump()
{
    Character::jump();
}

void Goku::moveLeft()
{
    Character::moveLeft();
}

void Goku::moveRight()
{
    Character::moveRight();
}

void Goku::stopMoving()
{
    Character::stopMoving();
}

void Goku::advance(int phase)
{
    Character::advance(phase);
}
