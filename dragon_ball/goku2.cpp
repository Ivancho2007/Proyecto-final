// goku2.cpp
#include "goku2.h"

Goku2::Goku2(QGraphicsItem *parent)
    : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/goku.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 480; // Ajustar según necesidad
}
