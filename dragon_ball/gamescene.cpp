#include "gamescene.h"
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QDebug>
#include "salud.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>


GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent),
    gameState(PLAYING),
    goku(nullptr),
    enemy(nullptr),
    secondEnemy(nullptr),
    backgroundItem(nullptr),
    gameTimer(nullptr),
    gameText(nullptr),
    gameOverlay(nullptr)
{
    setSceneRect(0, 0, 800, 600);
    setupLevel();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);
    gameTimer->start(16);
}

void GameScene::setupLevel()
{
    QPixmap fondo("C:/Users/IVAN/Downloads/fondo1.png");
    if (fondo.isNull()) {
        fondo = QPixmap(800, 600);
        fondo.fill(Qt::blue);
    }
    backgroundItem = new QGraphicsPixmapItem(fondo);
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);

    goku = new Goku();
    goku->setPos(50, 500 - goku->pixmap().height());
    addItem(goku);

    enemy = new Enemy("C:/Users/IVAN/Downloads/piccolo.png");
    enemy->setPos(650, 500 - enemy->pixmap().height());
    addItem(enemy);

    secondEnemy = new Enemy("C:/Users/IVAN/Downloads/nappa.png");
    secondEnemy->setPos(400, 500 - secondEnemy->pixmap().height());
    addItem(secondEnemy);


    QList<QRectF> platformRects = {
        QRectF(200, 290, 120, 15),
        QRectF(450, 380, 100, 15),
        QRectF(650, 310, 100, 15)
    };

    for (const QRectF &rect : platformRects) {
        QGraphicsRectItem *platform = new QGraphicsRectItem(rect);
        platform->setBrush(QColor(0, 200, 0, 200));
        platform->setPen(QPen(Qt::black, 1));
        platform->setZValue(1);
        addItem(platform);
        platforms.append(platform);
    }

    goku->groundLevel = 520;

    setupHealthBars();
    connect(goku, &Character::healthChanged, this, &GameScene::updateHealthBars);
    connect(enemy, &Character::healthChanged, this, &GameScene::updateHealthBars);
    connect(secondEnemy, &Character::healthChanged, this, &GameScene::updateHealthBars);
    spawnDragonBalls();
}

void GameScene::resizeBackground()
{
    if (backgroundItem) {
        QPixmap fondo("C:/Users/IVAN/Downloads/fondo1.png");
        if (fondo.isNull()) return;

        fondo = fondo.scaled(sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        backgroundItem->setPixmap(fondo);
        backgroundItem->setPos(0, 0);
    }
}

void GameScene::update()
{
    if (gameState != PLAYING) return;

    goku->advance(1);
    if (enemy) enemy->advance(1);
    if (secondEnemy) secondEnemy->advance(1);

    checkPlatformCollisions(goku);
    if (enemy) checkPlatformCollisions(enemy);
    if (secondEnemy) checkPlatformCollisions(secondEnemy);

    QRectF gokuRectCollision = goku->boundingRect().translated(goku->pos());
    for (DragonBall *ball : dragonBalls) {
        if (ball && gokuRectCollision.intersects(ball->boundingRect().translated(ball->pos()))) {
            emit ball->collected(ball->getHealthValue());
            removeItem(ball);
            dragonBalls.removeOne(ball);
            delete ball;
            break;
        }
    }

    if (goku->getHealth() <= 0 && gameState == PLAYING) {
        gameState = GAME_OVER;
    }

    if (enemy && enemy->getHealth() <= 0 &&
        secondEnemy && secondEnemy->getHealth() <= 0 &&
        gameState == PLAYING) {
        gameState = LEVEL_COMPLETED;
        showLevelComplete();
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (gameState != PLAYING) return;

    switch (event->key()) {
    case Qt::Key_A: goku->moveLeft(); break;
    case Qt::Key_D: goku->moveRight(); break;
    case Qt::Key_Space:
    case Qt::Key_W: goku->jump(); break;
    case Qt::Key_F:
        if (goku) {
            StoneAttack *stone = new StoneAttack(
                StoneAttack::GOKU_ATTACK,
                true,
                nullptr
                );
            stone->setPos(goku->x() + goku->pixmap().width(),
                          goku->y() + goku->pixmap().height()/3);
            stone->setZValue(50);
            addItem(stone);
        }
        break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        goku->stopMoving();
    }
}

void GameScene::setupHealthBars()
{
    int barWidth = 200;
    int barHeight = 20;
    int margin = 20;

    gokuHealthBackground = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    gokuHealthBackground->setPos(margin, margin);
    gokuHealthBackground->setBrush(Qt::gray);
    gokuHealthBackground->setPen(QPen(Qt::black, 2));
    gokuHealthBackground->setZValue(100);
    addItem(gokuHealthBackground);

    gokuHealthBar = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    gokuHealthBar->setPos(margin, margin);
    gokuHealthBar->setBrush(Qt::green);
    gokuHealthBar->setPen(Qt::NoPen);
    gokuHealthBar->setZValue(101);
    addItem(gokuHealthBar);

    piccoloHealthBackground = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    piccoloHealthBackground->setPos(width() - barWidth - margin, margin);
    piccoloHealthBackground->setBrush(Qt::gray);
    piccoloHealthBackground->setPen(QPen(Qt::black, 2));
    piccoloHealthBackground->setZValue(100);
    addItem(piccoloHealthBackground);

    piccoloHealthBar = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    piccoloHealthBar->setPos(width() - barWidth - margin, margin);
    piccoloHealthBar->setBrush(Qt::red);
    piccoloHealthBar->setPen(Qt::NoPen);
    piccoloHealthBar->setZValue(101);
    addItem(piccoloHealthBar);

    updateHealthBars();
}

void GameScene::updateHealthBars()
{
    qreal gokuHealthPercent = static_cast<qreal>(goku->getHealth()) / goku->getMaxHealth();
    gokuHealthBar->setRect(0, 0, 200 * gokuHealthPercent, 20);

    int combinedHealth = 0;
    int maxHealth = 0;
    if (enemy) {
        combinedHealth += enemy->getHealth();
        maxHealth += enemy->getMaxHealth();
    }
    if (secondEnemy) {
        combinedHealth += secondEnemy->getHealth();
        maxHealth += secondEnemy->getMaxHealth();
    }

    qreal avgHealthPercent = maxHealth > 0 ? static_cast<qreal>(combinedHealth) / maxHealth : 0;
    piccoloHealthBar->setRect(0, 0, 200 * avgHealthPercent, 20);
}

void GameScene::spawnDragonBalls()
{
    QList<QPointF> positions = {
        QPointF(220, 260),
        QPointF(470, 350),
        QPointF(670, 280)
    };

    for (int i = 0; i < positions.size(); ++i) {
        DragonBall::BallType type;
        switch(i) {
        case 0: type = DragonBall::ONE_STAR; break;
        case 1: type = DragonBall::TWO_STARS; break;
        case 2: type = DragonBall::THREE_STARS; break;
        }

        DragonBall *ball = new DragonBall(type);
        ball->setPos(positions[i]);
        addItem(ball);
        dragonBalls.append(ball);
        connect(ball, &DragonBall::collected, this, &GameScene::onDragonBallCollected);
    }
}

void GameScene::onDragonBallCollected(int healthRestored)
{
    goku->setHealth(qMin(goku->getHealth() + healthRestored, goku->getMaxHealth()));
    updateHealthBars();
    qDebug() << "Salud recuperada:" << healthRestored;
}

void GameScene::checkPlatformCollisions(Character* character)
{
    QRectF charRect = character->boundingRect().translated(character->pos());
    bool onPlatform = false;

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());

        if (charRect.intersects(platformRect) &&
            character->velocityY > 0 &&
            charRect.bottom() <= platformRect.top() + 5) {
            character->setPos(character->x(), platformRect.top() - charRect.height());
            character->velocityY = 0;
            character->isJumping = false;
            onPlatform = true;
            break;
        }
    }

    if (!onPlatform && character->y() + character->pixmap().height() >= character->groundLevel) {
        character->setPos(character->x(), character->groundLevel - character->pixmap().height());
        character->velocityY = 0;
        character->isJumping = false;
    }
}

void GameScene::showLevelComplete()
{
    gameTimer->stop();
    if (enemy) enemy->stopTimers();
    if (secondEnemy) secondEnemy->stopTimers();

    gameOverlay = new QGraphicsRectItem(0, 0, width(), height());
    gameOverlay->setBrush(QColor(0, 0, 0, 150));
    gameOverlay->setZValue(200);
    addItem(gameOverlay);

    gameText = new QGraphicsTextItem("¡Nivel 1 Completado!");
    gameText->setDefaultTextColor(Qt::white);
    gameText->setFont(QFont("Arial", 36, QFont::Bold));
    gameText->setPos(width()/2 - gameText->boundingRect().width()/2, height()/2 - 100);
    gameText->setZValue(201);
    addItem(gameText);

    QPushButton* backButton = new QPushButton("Volver al menú");
    backButton->setFixedSize(200, 50);
    backButton->setStyleSheet("font-size: 20px; background-color: #FFA500; color: white;");
    QGraphicsProxyWidget* proxyButton = addWidget(backButton);
    proxyButton->setPos(width()/2 - 100, height()/2 + 20);
    proxyButton->setZValue(202);
    connect(backButton, &QPushButton::clicked, this, &GameScene::returnToMenuRequested);
}

