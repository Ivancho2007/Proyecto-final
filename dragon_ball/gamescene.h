#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>

#include "goku.h"
#include "enemy.h"
#include "atack.h"
#include "salud.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum GameState { PLAYING, GAME_OVER, LEVEL_COMPLETED };

    explicit GameScene(QObject *parent = nullptr);
    void resizeBackground();
    const QList<QGraphicsRectItem*>& getPlatforms() const { return platforms; }
    GameState getGameState() const { return gameState; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void update();
    void onDragonBallCollected(int healthRestored);

private:
    void setupLevel();
    void showLevelComplete();

    void setupHealthBars();
    void updateHealthBars();
    void checkPlatformCollisions(Character* character);
    void spawnDragonBalls();

    // Personajes
    Goku *goku;
    Enemy *enemy;
    Enemy *secondEnemy;

    // Salud
    QGraphicsRectItem* gokuHealthBar;
    QGraphicsRectItem* gokuHealthBackground;
    QGraphicsRectItem* piccoloHealthBar;
    QGraphicsRectItem* piccoloHealthBackground;

    // Otros
    QGraphicsPixmapItem *backgroundItem;
    QList<QGraphicsRectItem*> platforms;
    QList<DragonBall*> dragonBalls;
    QTimer *gameTimer;
    GameState gameState;
    QGraphicsTextItem* gameText;
    QGraphicsRectItem* gameOverlay;
};

#endif // GAMESCENE_H
