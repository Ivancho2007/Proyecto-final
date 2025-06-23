#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QTimer>

class Enemy : public Character
{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent = nullptr);
    void advance(int phase) override;



public slots:
    void moveTowardsTarget();
    void decideAction();
    void attack();


private:
    QTimer *aiTimer;
    QTimer *attackTimer;
    QGraphicsItem* target;
    int attackCooldown = 2000; // 2 segundos entre ataques
};

#endif // ENEMY_H
