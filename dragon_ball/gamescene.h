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
    void checkPlatformCollisions(Character* character);
    void spawnDragonBalls();
    QList<DragonBall*> dragonBalls;
    GameState gameState;

    QGraphicsTextItem* gameText;
    QGraphicsRectItem* gameOverlay;
};

#endif // GAMESCENE_H