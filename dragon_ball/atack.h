#ifndef ATACK_H
#define ATACK_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class StoneAttack : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum AttackOwner { GOKU_ATTACK, PICCOLO_ATTACK };

    StoneAttack(AttackOwner owner, bool movingRight, QGraphicsItem *parent = nullptr);
    virtual ~StoneAttack() override; // Declaración explícita del destructor

public slots:
    void move();

private:
    void removeAttack();

    QTimer *moveTimer;
    int damage;
    AttackOwner owner;
    bool movingRight;
};

#endif // ATACK_H


