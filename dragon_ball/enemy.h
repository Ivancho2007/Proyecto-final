#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character
{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent = nullptr);
};

#endif // ENEMY_H
