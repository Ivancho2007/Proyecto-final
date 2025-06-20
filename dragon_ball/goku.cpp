#include "goku.h"

Goku::Goku(QGraphicsItem *parent) : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/goku.png").scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 405; // Asegura que coincida con el fondo
    setY(groundLevel); // Coloca a Goku sobre el "suelo"
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
