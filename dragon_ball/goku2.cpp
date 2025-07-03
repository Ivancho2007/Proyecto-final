#include "goku2.h"
#include "atack.h"
#include <QGraphicsScene>

Goku2::Goku2(QGraphicsItem *parent)
    : Character(parent)
{
    setPixmap(QPixmap("C:/Users/IVAN/Downloads/goku.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    groundLevel = 480;
}

void Goku2::launchKamehameha()
{
    StoneAttack* beam = new StoneAttack(StoneAttack::GOKU_ATTACK, true);
    beam->setPixmap(QPixmap("C:/Users/IVAN/Downloads/kamehameha.png").scaled(80, 40));
    beam->setPos(x() + pixmap().width(), y() + pixmap().height()/3);
    scene()->addItem(beam);
}
