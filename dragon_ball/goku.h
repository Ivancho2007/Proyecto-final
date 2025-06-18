#ifndef GOKU_H
#define GOKU_H

#include "character.h"

class Goku : public Character
{
    Q_OBJECT
public:
    explicit Goku(QGraphicsItem *parent = nullptr);

    void jump() override;
    void moveLeft() override;
    void moveRight() override;
    void stopMoving() override;
    void advance(int phase) override;
};

#endif // GOKU_H
