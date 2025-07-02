#include "gamescenelevel2.h"
#include "atack.h"
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QGraphicsTextItem>

GameSceneLevel2::GameSceneLevel2(QObject *parent)
    : QGraphicsScene(parent), gameOver(false)
{
    setSceneRect(0, 0, 800, 600);
    setupLevel();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameSceneLevel2::update);
    gameTimer->start(16);

    rainTimer = new QTimer(this);
    connect(rainTimer, &QTimer::timeout, this, &GameSceneLevel2::spawnStoneRain);
    rainTimer->start(2000);
}

void GameSceneLevel2::setupLevel()
{
    QPixmap fondo("C:/Users/IVAN/Downloads/fondo2.png");
    if (fondo.isNull()) fondo.fill(Qt::cyan);
    backgroundItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);

    spawnPlatforms();

    goku = new Goku2();
    goku->groundLevel = 540;
    goku->setPos(100, goku->groundLevel - goku->pixmap().height());
    addItem(goku);

    enemy = new Enemy2("C:/Users/IVAN/Downloads/majinbuu.png");
    enemy->groundLevel = 520;
    enemy->setPos(600, enemy->groundLevel - enemy->pixmap().height());
    addItem(enemy);
}

void GameSceneLevel2::spawnPlatforms()
{
    // Plataformas visibles
    QList<QRectF> platformRects = {
        QRectF(100, 400, 150, 20),
        QRectF(350, 300, 150, 20),
        QRectF(600, 400, 150, 20)
    };

    for (const QRectF &rect : platformRects) {
        QGraphicsRectItem *platform = new QGraphicsRectItem(rect);
        platform->setBrush(QColor(255, 255, 255, 180));
        platform->setPen(QPen(Qt::gray, 1));
        platform->setZValue(1);
        addItem(platform);
        platforms.append(platform);
    }

    // Suelo invisible
    QGraphicsRectItem *ground = new QGraphicsRectItem(0, 560, 800, 40);
    ground->setBrush(Qt::transparent);
    ground->setPen(Qt::NoPen);
    addItem(ground);
    platforms.append(ground);
}

void GameSceneLevel2::resizeBackground()
{
    if (backgroundItem) {
        QPixmap fondo("C:/Users/IVAN/Downloads/fondo2.png");
        if (fondo.isNull()) return;
        fondo = fondo.scaled(sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        backgroundItem->setPixmap(fondo);
    }
}

void GameSceneLevel2::update()
{
    if (gameOver) return;

    goku->advance(1);
    enemy->advance(1);
    checkPlatformCollisions();

    if (playerFell()) {
        gameOver = true;
        showGameOver();
    }
}

bool GameSceneLevel2::playerFell() const
{
    return goku && goku->y() > height() + 50;
}

void GameSceneLevel2::checkPlatformCollisions()
{
    // Para Goku
    QRectF gokuFeetRect(
        goku->x() + 15, // Margen lateral pequeño
        goku->y() + goku->pixmap().height() - 20, // Justo en los pies
        goku->pixmap().width() - 25, // Ancho reducido
        20 // Altura del área de detección
        );

    bool gokuOnPlatform = false;

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());

        // Detección más precisa para cuando está cayendo
        if (gokuFeetRect.intersects(platformRect) && goku->velocityY > 0) {
            goku->setPos(goku->x(), platformRect.top() - goku->pixmap().height());
            goku->velocityY = 0;
            goku->isJumping = false;
            gokuOnPlatform = true;
            break;
        }
    }

    // Si no está en plataforma pero está en el suelo
    if (!gokuOnPlatform) {
        QRectF groundRect(0, 560, 800, 40);
        if (gokuFeetRect.intersects(groundRect) && goku->velocityY > 0) {
            goku->setPos(goku->x(), groundRect.top() - goku->pixmap().height());
            goku->velocityY = 0;
            goku->isJumping = false;
        }
    }

    // Para el enemigo (misma lógica)
    QRectF enemyFeetRect(
        enemy->x() + 5,
        enemy->y() + enemy->pixmap().height() - 10,
        enemy->pixmap().width() - 10,
        12
        );

    bool enemyOnPlatform = false;

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());

        if (enemyFeetRect.intersects(platformRect) && enemy->velocityY > 0) {
            enemy->setPos(enemy->x(), platformRect.top() - enemy->pixmap().height());
            enemy->velocityY = 0;
            enemy->isJumping = false;
            enemyOnPlatform = true;
            break;
        }
    }

    // Suelo para el enemigo
    if (!enemyOnPlatform) {
        QRectF groundRect(0, 560, 800, 40);
        if (enemyFeetRect.intersects(groundRect) && enemy->velocityY > 0) {
            enemy->setPos(enemy->x(), groundRect.top() - enemy->pixmap().height());
            enemy->velocityY = 0;
            enemy->isJumping = false;
        }
    }
}

void GameSceneLevel2::showGameOver()
{
    QGraphicsTextItem* lostText = new QGraphicsTextItem("¡Caíste!");
    lostText->setDefaultTextColor(Qt::red);
    lostText->setFont(QFont("Arial", 36, QFont::Bold));
    lostText->setZValue(10);
    lostText->setPos(width()/2 - lostText->boundingRect().width()/2, height()/2 - 50);
    addItem(lostText);

    gameTimer->stop();
    rainTimer->stop();
    enemy->stopTimers();
}

void GameSceneLevel2::keyPressEvent(QKeyEvent *event)
{
    if (gameOver) return;

    switch (event->key()) {
    case Qt::Key_A: goku->moveLeft(); break;
    case Qt::Key_D: goku->moveRight(); break;
    case Qt::Key_W:
    case Qt::Key_Space: goku->jump(); break;
    case Qt::Key_F:
        if (goku) {
            StoneAttack *stone = new StoneAttack(StoneAttack::GOKU_ATTACK, true);
            stone->setPos(goku->x() + goku->pixmap().width(), goku->y() + goku->pixmap().height()/3);
            addItem(stone);
        }
        break;
    }
}

void GameSceneLevel2::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D)
        goku->stopMoving();
}

void GameSceneLevel2::spawnStoneRain()
{
    int x = QRandomGenerator::global()->bounded(50, 750);
    StoneAttack *rainStone = new StoneAttack(StoneAttack::PICCOLO_ATTACK, false);
    rainStone->setPixmap(QPixmap("C:/Users/IVAN/Downloads/lluvia.png").scaled(30, 30));
    rainStone->setPos(x, 0);
    rainStone->setZValue(5);
    addItem(rainStone);
}
