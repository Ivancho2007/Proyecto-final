#ifndef ENEMY2_H
#define ENEMY2_H

#include "character.h"
#include <QTimer>

class Enemy2 : public Character
{
    Q_OBJECT
public:
    explicit Enemy2(const QString& spritePath, QGraphicsItem *parent = nullptr);
    void stopTimers();

public slots:
    void decideAction();
    void attack();
    void specialAttack();
    void ultraAttack();
    void ultimateAttack();

private:
    QGraphicsItem* target = nullptr;
    QTimer *aiTimer;
    QTimer *attackTimer;
    QTimer *specialTimer;
    QTimer *ultraTimer;
    QTimer *ultimateTimer;
    int attackCooldown = 1000;
};

#endif // ENEMY2_H
