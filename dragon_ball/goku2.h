#ifndef GOKU2_H
#define GOKU2_H

#include "character.h"

class Goku2 : public Character
{
    Q_OBJECT
public:
    explicit Goku2(QGraphicsItem *parent = nullptr);
    void launchKamehameha();
};

#endif // GOKU2_H
