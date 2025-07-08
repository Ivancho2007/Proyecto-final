#include "enemy2.h"
#include "goku2.h"
#include "atack.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include "gamescenelevel2.h"

Enemy2::Enemy2(const QString& spritePath, QGraphicsItem *parent)
    : Character(parent)
{
    setPixmap(QPixmap(spritePath).scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 560;
    moveSpeed = 5;
    jumpSpeed = 12;
    gravity = 0.6;

    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Enemy2::decideAction);
    aiTimer->start(200);

    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &Enemy2::attack);

    specialTimer = new QTimer(this);
    connect(specialTimer, &QTimer::timeout, this, &Enemy2::specialAttack);
    specialTimer->start(2000); // cada 5 segundos lanza súper ataque
}

void Enemy2::decideAction()
{
    GameSceneLevel2* gameScene = qobject_cast<GameSceneLevel2*>(scene());
    if (!gameScene) return;

    if (!target) {
        for (QGraphicsItem* item : gameScene->items()) {
            if (dynamic_cast<Goku2*>(item)) {
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

        if (QRandomGenerator::global()->bounded(100) < 10 && !isJumping) {
            jump();
        }
    }
}

void Enemy2::attack()
{
    if (!target || !scene()) return;

    bool attackRight = (x() < target->x());
    StoneAttack* stone = new StoneAttack(StoneAttack::PICCOLO_ATTACK, attackRight);
    stone->setPixmap(QPixmap("C:/Users/IVAN/Downloads/stone.png").scaled(40, 40));

    stone->setPos(
        attackRight ? x() + pixmap().width() : x() - stone->pixmap().width(),
        y() + pixmap().height() / 2
        );
    scene()->addItem(stone);
    attackTimer->stop();
}

void Enemy2::specialAttack()
{
    if (!target || !scene()) return;

    bool attackRight = (x() < target->x());
    StoneAttack* special = new StoneAttack(StoneAttack::PICCOLO_ATTACK, attackRight);
    special->setPixmap(QPixmap("C:/Users/IVAN/Downloads/superbola.png").scaled(80, 60));

    special->setPos(
        attackRight ? x() + pixmap().width() : x() - special->pixmap().width(),
        y() + 20
        );
    scene()->addItem(special);
}

void Enemy2::stopTimers()
{
    if (aiTimer) aiTimer->stop();
    if (attackTimer) attackTimer->stop();
    if (specialTimer) specialTimer->stop();
}
