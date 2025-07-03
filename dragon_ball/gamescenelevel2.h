#ifndef GAMESCENELEVEL2_H
#define GAMESCENELEVEL2_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include "goku2.h"
#include "enemy2.h"

class GameSceneLevel2 : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameSceneLevel2(QObject *parent = nullptr);
    void resizeBackground();
    bool playerFell() const;
    QList<QGraphicsRectItem*> getPlatforms() const { return platforms; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void update();
    void spawnStoneRain();

signals:
    void returnToMenuRequested();

private:
    void setupLevel();
    void checkPlatformCollisions();
    void spawnPlatforms();
    void showGameOver();

    QGraphicsPixmapItem *backgroundItem;
    QList<QGraphicsRectItem*> platforms;

    Goku2 *goku;
    Enemy2 *enemy;

    QTimer *gameTimer;
    QTimer *rainTimer;
    bool gameOver = false;

    QGraphicsRectItem *gokuHealthBar;
    QGraphicsRectItem *gokuHealthBackground;
    QGraphicsRectItem *enemyHealthBar;
    QGraphicsRectItem *enemyHealthBackground;
    void setupHealthBars();
    void updateHealthBars();


};

#endif // GAMESCENELEVEL2_H
