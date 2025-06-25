#ifndef SALUD_H
#define SALUD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class DragonBall : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum BallType { ONE_STAR, TWO_STARS, THREE_STARS };

    DragonBall(BallType type, QGraphicsItem *parent = nullptr);
    int getHealthValue() const;

signals:
    void collected(int healthRestored);

private:
    BallType ballType;
    int healthValue;
    QTimer *floatTimer;

    void setupAppearance();
    void setupAnimation();
};

#endif // SALUD_H