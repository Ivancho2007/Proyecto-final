#include "salud.h"
#include <QGraphicsScene>
#include <QPropertyAnimation>

DragonBall::DragonBall(BallType type, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), ballType(type)
{

    switch(ballType) {
    case ONE_STAR: healthValue = 5; break;
    case TWO_STARS: healthValue = 10; break;
    case THREE_STARS: healthValue = 8; break;
    }

    setupAppearance();
    setupAnimation();
}

void DragonBall::setupAppearance()
{
    QString imagePath;
    switch(ballType) {
    case ONE_STAR: imagePath = "C:/Users/IVAN/Downloads/esfera1.png"; break;
    case TWO_STARS: imagePath = "C:/Users/IVAN/Downloads/esfera2.png"; break;
    case THREE_STARS: imagePath = "C:/Users/IVAN/Downloads/esfera3.png"; break;
    }

    QPixmap pix(imagePath);
    setPixmap(pix.scaled(30, 30, Qt::KeepAspectRatio));
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setZValue(10);
}

void DragonBall::setupAnimation()
{
    // Animación de flotación
    QPropertyAnimation *floatAnim = new QPropertyAnimation(this, "y");
    floatAnim->setDuration(2000);
    floatAnim->setStartValue(y() - 5);
    floatAnim->setEndValue(y() + 5);
    floatAnim->setEasingCurve(QEasingCurve::InOutQuad);
    floatAnim->setLoopCount(-1); // Infinito
    floatAnim->start();

    // Rotación
    QPropertyAnimation *rotateAnim = new QPropertyAnimation(this, "rotation");
    rotateAnim->setDuration(5000);
    rotateAnim->setStartValue(0);
    rotateAnim->setEndValue(360);
    rotateAnim->setLoopCount(-1);
    rotateAnim->start();
}

int DragonBall::getHealthValue() const {
    return healthValue;
}
