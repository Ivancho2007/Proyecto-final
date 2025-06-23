#include "enemy.h"
#include "goku.h"
#include "atack.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

Enemy::Enemy(QGraphicsItem *parent) : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/piccolo.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 500;
    setY(groundLevel - pixmap().height()+30);
    moveSpeed = 3;
    jumpSpeed = 12;
    gravity = 0.6;

    maxHealth = 100;
    currentHealth = 100;
    moveSpeed = 5; // Más lento que Goku

    // Timers para IA
    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Enemy::decideAction);
    aiTimer->start(200); // Toma decisiones cada 500ms

    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &Enemy::attack);
    attackCooldown = 1000; // 1 segundo entre ataques (antes 2000ms)
}

void Enemy::advance(int phase)
{
    if (!phase) return;

    Character::advance(phase); // Usa la lógica base de movimiento
}

void Enemy::decideAction()
{
    // Buscar a Goku en la escena
    if (!target) {
        foreach (QGraphicsItem *item, scene()->items()) {
            if (dynamic_cast<Goku*>(item)) {
                target = item;
                break;
            }
        }
    }

    if (target) {
        // Movimiento hacia Goku
        if (x() < target->x() - 50) {
            moveRight();
        }
        else if (x() > target->x() + 50) {
            moveLeft();
        }
        else {
            stopMoving();
            // Posición adecuada para atacar
            if (!attackTimer->isActive()) {
                attackTimer->start(attackCooldown);
            }
        }

        // Salto aleatorio
        if (QRandomGenerator::global()->bounded(100) < 10 && !isJumping) {
            jump();
        }
    }
}

void Enemy::attack()
{
    if (!target || !scene()) return;

    bool attackRight = (x() < target->x()); // Determinar dirección

    StoneAttack *stone = new StoneAttack(
        StoneAttack::PICCOLO_ATTACK,
        attackRight,
        nullptr
        );

    // Posición inicial basada en dirección
    if (attackRight) {
        stone->setPos(x() + pixmap().width(), y() + pixmap().height()/2);
    } else {
        stone->setPos(x() - 30, y() + pixmap().height()/2);
    }

    stone->setZValue(50);
    scene()->addItem(stone);

    attackTimer->stop();
}

void Enemy::moveTowardsTarget() {
    if (!target) return;

    if (x() < target->x() - 5) {
        moveRight();
    } else if (x() > target->x() + 5) {
        moveLeft();
    } else {
        stopMoving();
    }
}

