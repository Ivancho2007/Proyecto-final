#ifndef GAMESCENELEVEL2_H
#define GAMESCENELEVEL2_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include "goku2.h"
#include "enemy2.h"

class GameSceneLevel2 : public QGraphicsScene
{
    Q_OBJECT

    enum GameState { PLAYING, GAME_OVER, LEVEL_COMPLETED };

public:
    explicit GameSceneLevel2(QObject *parent = nullptr);
    void resizeBackground();
    bool playerFell() const;
    QList<QGraphicsRectItem*> getPlatforms() const { return platforms; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void update();
    void spawnStoneRain();
    void returnToMenu();

signals:
    void returnToMenuRequested();
    void goToMainMenu();

private:
    void setupLevel();
    void checkPlatformCollisions();
    void spawnPlatforms();
    void showGameOver();
    void showVictory();
    void setupHealthBars();
    void updateHealthBars();

    QGraphicsPixmapItem *backgroundItem;
    QList<QGraphicsRectItem*> platforms;

    Goku2 *goku;
    Enemy2 *enemy;

    QTimer *gameTimer;
    QTimer *rainTimer;
    QTimer *countdownTimer;
    int timeLeft = 120;

    GameState gameState = PLAYING;

    QGraphicsRectItem *gokuHealthBar;
    QGraphicsRectItem *gokuHealthBackground;
    QGraphicsRectItem *enemyHealthBar;
    QGraphicsRectItem *enemyHealthBackground;
    QGraphicsRectItem* backButtonItem = nullptr;

    QGraphicsTextItem *timerText = nullptr;
};

#endif // GAMESCENELEVEL2_H
