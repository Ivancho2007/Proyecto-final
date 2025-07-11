#include "atack.h"
#include "enemy.h"
#include "enemy2.h"
#include "goku.h"
#include "goku2.h"
#include <QGraphicsScene>
#include <QDebug>

StoneAttack::StoneAttack(AttackOwner owner, bool movingRight, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), owner(owner), movingRight(movingRight)
{
    if (owner == GOKU_ATTACK) {
        setPixmap(QPixmap("C:/Users/IVAN/Downloads/power.png").scaled(40, 40));
        damage = 12;
    } else {
        setPixmap(QPixmap("C:/Users/IVAN/Downloads/stone.png").scaled(40, 40));
        damage = 16;
    }

    if (!movingRight) {
        setTransform(QTransform::fromScale(-1, 1));
    }

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &StoneAttack::move);
    moveTimer->start(30);
}

void StoneAttack::setDamage(int value) {
    damage = value;
}

int StoneAttack::getDamage() const {
    return damage;
}

void StoneAttack::move()
{
    setPos(x() + (movingRight ? 20 : -20), y());

    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        if (owner == GOKU_ATTACK &&
            (dynamic_cast<Enemy*>(item) || dynamic_cast<Enemy2*>(item))) {
            Character* enemy = dynamic_cast<Character*>(item);
            if (enemy) {
                enemy->decreaseHealth(damage);
                removeAttack();
                return;
            }
        }
        else if (owner == PICCOLO_ATTACK &&
                 (dynamic_cast<Goku*>(item) || dynamic_cast<Goku2*>(item))) {
            Character* goku = dynamic_cast<Character*>(item);
            if (goku) {
                goku->decreaseHealth(damage);
                removeAttack();
                return;
            }
        }
    }

    if (x() > scene()->width() + 100 || x() < -100) {
        removeAttack();
    }
}

void StoneAttack::removeAttack()
{
    scene()->removeItem(this);
    delete this;
}

StoneAttack::~StoneAttack()
{
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
    qDebug() << "StoneAttack destruido correctamente";
}

