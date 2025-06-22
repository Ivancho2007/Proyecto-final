#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem> // Cambiado de QGraphicsPixmapItem
#include <QTimer>
#include "goku.h"
#include "enemy.h"
#include "StoneAttack.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    void resizeBackground();

    const QList<QGraphicsRectItem*>& getPlatforms() const { return platforms; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private slots:
    void update();

private:
    void setupLevel();
    // Barras de vida
    QGraphicsRectItem* gokuHealthBar;
    QGraphicsRectItem* gokuHealthBackground;
    QGraphicsRectItem* piccoloHealthBar;
    QGraphicsRectItem* piccoloHealthBackground;

    void setupHealthBars();
    void updateHealthBars();
    Goku *goku;
    Enemy *enemy;
    QGraphicsPixmapItem *backgroundItem;
    QList<QGraphicsRectItem*> platforms;
    QTimer *gameTimer;
};

#endif // GAMESCENE_H

