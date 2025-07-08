#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QTimer>

class GameScene;

class Enemy : public Character
{
    Q_OBJECT
public:
    explicit Enemy(const QString& spritePath, QGraphicsItem *parent = nullptr);
    void advance(int phase) override;
    void stopTimers();

public slots:
    void moveTowardsTarget();
    void decideAction();
    void attack();

private:
    QGraphicsItem* target = nullptr;
    QTimer *aiTimer;
    QTimer *attackTimer;
    int attackCooldown = 1000;
};

#endif // ENEMY_H

