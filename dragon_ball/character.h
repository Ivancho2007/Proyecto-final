#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QGraphicsPixmapItem>
class GameScene;

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Character(QGraphicsItem *parent = nullptr);
    virtual ~Character() = default;

    int health = 100;
    bool isRunning = false;
    bool facingRight = true;

    //barras de vida
    int maxHealth = 100;
    int currentHealth = 100;

    void decreaseHealth(int amount);
    int getHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }

    // Movimiento horizontal
    qreal velocityX = 0;
    qreal moveSpeed = 5;

    // Movimiento vertical (salto parabólico)
    bool isJumping = false;
    qreal velocityY = 0;
    qreal gravity = 0.5;
    qreal jumpSpeed = 10;
    qreal groundLevel = 405;  // Puedes ajustar este valor

    virtual void jump();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stopMoving();
    virtual void basicAttack();
    virtual void advance(int phase) override;
    virtual bool checkPlatformCollision(qreal x, qreal y, const QList<QGraphicsRectItem*>& platforms);

signals:
    void healthChanged();
};

#endif // CHARACTER_H
