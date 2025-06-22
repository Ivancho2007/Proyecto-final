#ifndef STONEATTACK_H
#define STONEATTACK_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class StoneAttack : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    StoneAttack(QGraphicsItem *parent = nullptr);
    ~StoneAttack();

public slots:
    void move();

private:
    QTimer *moveTimer;
    int damage = 15;
};

#endif // STONEATTACK_H
