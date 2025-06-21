#include "character.h"
#include <QDebug>
#include <QGraphicsScene>
#include "gamescene.h"

Character::Character(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/goku.png").scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Character::jump()
{
    if (!isJumping) {
        velocityY = -14;  // ← Aumenta el impulso del salto
        isJumping = true;
    }
}

void Character::moveLeft()
{
    velocityX = -moveSpeed;
    facingRight = false;
    isRunning = true;
}

void Character::moveRight()
{
    velocityX = moveSpeed;
    facingRight = true;
    isRunning = true;
}

void Character::stopMoving()
{
    velocityX = 0;
    isRunning = false;
}

void Character::basicAttack()
{
    qDebug() << "Basic attack performed";
}


bool Character::checkPlatformCollision(qreal x, qreal y, const QList<QGraphicsRectItem*>& platforms)
{
    QRectF characterRect(x, y, pixmap().width(), pixmap().height());

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());

        if (characterRect.intersects(platformRect) &&
            y + pixmap().height() <= platformRect.top() + 5 &&
            velocityY > 0) {
            return true;
        }
    }
    return false;
}

void Character::advance(int phase)
{
    if (!phase) return;

    // Movimiento horizontal
    qreal newX = x() + velocityX;
    if (newX < 0) newX = 0;
    if (newX > scene()->width() - pixmap().width()) {
        newX = scene()->width() - pixmap().width();
    }

    // Movimiento vertical
    velocityY += 0.4;
    qreal newY = y() + velocityY;

    // Sistema mejorado de detección de plataformas
    bool onPlatform = false;
    GameScene* gameScene = qobject_cast<GameScene*>(scene());

    if (gameScene && velocityY > 0) { // Solo cuando está cayendo
        // Área de detección más precisa (justo en los pies del personaje)
        QRectF feetRect(
            newX + 2, // Pequeño margen lateral
            newY + pixmap().height() - 10, // Justo en la base del sprite
            pixmap().width() - 4, // Ancho reducido para mejor precisión
            12 // Altura pequeña del área de detección
            );

        for (QGraphicsRectItem* platform : gameScene->getPlatforms()) {
            QRectF platformRect = platform->rect().translated(platform->pos());

            // Detección precisa cuando los pies tocan la plataforma
            if (feetRect.intersects(platformRect)) {
                // Ajuste exacto de la posición
                newY = platformRect.top() - pixmap().height() + 23;
                velocityY = 0;
                isJumping = false;
                onPlatform = true;
                break;
            }
        }
    }
    else if (newY >= groundLevel) {
        newY = groundLevel - pixmap().height();
        velocityY = 0;
        isJumping = false;
    }

    setPos(newX, newY);
}
void Character::decreaseHealth(int amount)
{
    currentHealth = qMax(0, currentHealth - amount);
    qDebug() << "Health decreased to:" << currentHealth;
}
