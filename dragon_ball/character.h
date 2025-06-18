#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Character(QGraphicsItem *parent = nullptr);
    virtual ~Character() = default;

    int health = 100;
    bool isRunning = false;
    bool facingRight = true;

    qreal velocityX = 0;
    qreal moveSpeed = 5;

    virtual void jump();  // aún sin salto funcional
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stopMoving();
    virtual void basicAttack();
    virtual void advance(int phase) override;
};

#endif // CHARACTER_H

