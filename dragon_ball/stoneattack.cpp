#include "StoneAttack.h"
#include <QGraphicsScene>
#include <QDebug>
#include "enemy.h"

StoneAttack::StoneAttack(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    // Configurar la apariencia de la piedra
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/power.png").scaled(60, 60, Qt::KeepAspectRatio));

    // Configurar el timer para movimiento
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &StoneAttack::move);
    moveTimer->start(50); // Mover cada 50ms
}

StoneAttack::~StoneAttack()
{
    delete moveTimer;
}

void StoneAttack::move()
{
    // Mover la piedra hacia la derecha
    setPos(x() + 15, y());

    // Verificar colisión con Piccolo
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            enemy->decreaseHealth(damage);
            scene()->removeItem(this);
            delete this;
            return;
        }
    }

    // Eliminar si sale de la pantalla
    if (x() > scene()->width()) {
        scene()->removeItem(this);
        delete this;
    }
}
