#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QTimer>
class GameScene;

class Enemy : public Character
{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent = nullptr);
    void advance(int phase) override;
    QTimer *aiTimer;
    QTimer *attackTimer;
    void stopTimers();

public slots:
    void moveTowardsTarget();
    void decideAction();
    void attack();

private:

    QGraphicsItem* target;
    int attackCooldown = 2000;
};
#endif // ENEMY_H
