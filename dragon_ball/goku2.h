// goku2.h
#ifndef GOKU2_H
#define GOKU2_H

#include "character.h"

class Goku2 : public Character
{
    Q_OBJECT
public:
    explicit Goku2(QGraphicsItem *parent = nullptr);
};

#endif // GOKU2_H
