#include "enemy.h"
#include "goku.h"
#include "atack.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include "gamescene.h"

Enemy::Enemy(const QString& spritePath, QGraphicsItem *parent)
    : Character(parent)
{
    setPixmap(QPixmap(spritePath).scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 500;
    moveSpeed = 5;
    jumpSpeed = 12;
    gravity = 0.6;
    maxHealth = 100;
    currentHealth = 100;

    // Inicializar timers
    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Enemy::decideAction);
    aiTimer->start(200);

    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &Enemy::attack);
    attackCooldown = 1000;
}

void Enemy::advance(int phase)
{
    if (!phase) return;
    Character::advance(phase);
}

void Enemy::decideAction()
{
    GameScene* gameScene = qobject_cast<GameScene*>(this->scene());
    if (!gameScene || gameScene->getGameState() != GameScene::PLAYING) {
        stopMoving();
        return;
    }

    if (!target) {
        for (QGraphicsItem *item : gameScene->items()) {
            if (dynamic_cast<Goku*>(item)) {
                target = item;
                break;
            }
        }
    }

    if (target) {
        if (x() < target->x() - 50) {
            moveRight();
        } else if (x() > target->x() + 50) {
            moveLeft();
        } else {
            stopMoving();
            if (!attackTimer->isActive()) {
                attackTimer->start(attackCooldown);
            }
        }

        // Saltar aleatoriamente
        if (QRandomGenerator::global()->bounded(100) < 10 && !isJumping) {
            jump();
        }
    }
}

void Enemy::attack()
{
    GameScene* gameScene = qobject_cast<GameScene*>(this->scene());
    if (!gameScene || gameScene->getGameState() != GameScene::PLAYING) return;
    if (!target || !scene()) return;

    bool attackRight = (x() < target->x());

    StoneAttack *stone = new StoneAttack(
        StoneAttack::PICCOLO_ATTACK,
        attackRight
        );

    if (attackRight) {
        stone->setPos(x() + pixmap().width(), y() + pixmap().height() / 2);
    } else {
        stone->setPos(x() - 30, y() + pixmap().height() / 2);
    }

    stone->setZValue(50);
    scene()->addItem(stone);

    attackTimer->stop();
}

void Enemy::moveTowardsTarget()
{
    if (!target) return;

    if (x() < target->x() - 5) {
        moveRight();
    } else if (x() > target->x() + 5) {
        moveLeft();
    } else {
        stopMoving();
    }
}

void Enemy::stopTimers()
{
    aiTimer->stop();
    attackTimer->stop();
}

