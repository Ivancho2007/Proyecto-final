#include "atack.h"
#include "enemy.h"
#include "goku.h"
#include <QGraphicsScene>
#include <QDebug>
#include "enemy2.h"
#include "goku2.h"


StoneAttack::StoneAttack(AttackOwner owner, bool movingRight, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), owner(owner), movingRight(movingRight)
{
    // Configurar imagen y daño según el dueño
    if (owner == GOKU_ATTACK) {
        setPixmap(QPixmap("C:/Users/IVAN/Downloads/power.png").scaled(40, 40));
        damage = 12;
    } else {
        setPixmap(QPixmap("C:/Users/IVAN/Downloads/stone.png").scaled(40, 40));
        damage = 16;
    }

    // Invertir imagen si va hacia la izquierda
    if (!movingRight) {
        setTransform(QTransform::fromScale(-1, 1));
    }

    // Configurar timer de movimiento
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &StoneAttack::move);
    moveTimer->start(30); // Mover cada 30ms
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
    // Limpiar el timer si aún existe
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
    qDebug() << "StoneAttack destruido correctamente";
}

