#include "gamescenelevel2.h"
#include "atack.h"
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

GameSceneLevel2::GameSceneLevel2(QObject *parent)
    : QGraphicsScene(parent), gameState(PLAYING)
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
    QPixmap fondo("C:/Users/IVAN/Downloads/fondo3.png");
    if (fondo.isNull()) fondo.fill(Qt::cyan);
    backgroundItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);

    spawnPlatforms();

    goku = new Goku2();
    goku->groundLevel = 540;
    goku->setPos(100, goku->groundLevel - goku->pixmap().height());
    addItem(goku);

    enemy = new Enemy2("C:/Users/IVAN/Downloads/piccolo2.png");
    enemy->groundLevel = 560;
    enemy->setPos(600, enemy->groundLevel - enemy->pixmap().height() - 20);
    addItem(enemy);

    setupHealthBars();
    connect(goku, &Character::healthChanged, this, &GameSceneLevel2::updateHealthBars);
    connect(enemy, &Character::healthChanged, this, &GameSceneLevel2::updateHealthBars);

    // Crear temporizador visible
    timerText = new QGraphicsTextItem("02:00");
    timerText->setDefaultTextColor(Qt::white);
    timerText->setFont(QFont("Consolas", 20, QFont::Bold));
    timerText->setZValue(105);
    timerText->setPos((width() - timerText->boundingRect().width()) / 2, 20);
    addItem(timerText);

    // Iniciar cuenta regresiva
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, [=]() {
        if (--timeLeft <= 0) {
            countdownTimer->stop();
            gameState = GAME_OVER;
            showGameOver();
            return;
        }

        int minutes = timeLeft / 60;
        int seconds = timeLeft % 60;
        timerText->setPlainText(QString("%1:%2")
                                    .arg(minutes, 2, 10, QLatin1Char('0'))
                                    .arg(seconds, 2, 10, QLatin1Char('0')));
    });
    countdownTimer->start(1000);
}

void GameSceneLevel2::spawnPlatforms()
{
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

    QGraphicsRectItem *ground = new QGraphicsRectItem(0, 560, 800, 40);
    ground->setBrush(Qt::transparent);
    ground->setPen(Qt::NoPen);
    addItem(ground);
    platforms.append(ground);
}

void GameSceneLevel2::resizeBackground()
{
    if (backgroundItem) {
        QPixmap fondo("C:/Users/IVAN/Downloads/fondo3.png");
        if (fondo.isNull()) return;
        fondo = fondo.scaled(sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        backgroundItem->setPixmap(fondo);
    }
}

void GameSceneLevel2::update()
{
    if (gameState != PLAYING) return;

    goku->advance(1);
    enemy->advance(1);
    checkPlatformCollisions();

    if (playerFell()) {
        gameState = GAME_OVER;
        showGameOver();
    }
    else if (enemy && enemy->getHealth() <= 0) {
        gameState = LEVEL_COMPLETED;
        showVictory();
    }
    else if (goku && goku->getHealth() <= 0 && gameState == PLAYING) {
        gameState = GAME_OVER;

        if (gameTimer) gameTimer->stop();
        if (rainTimer) rainTimer->stop();
        if (countdownTimer) countdownTimer->stop();
        if (enemy) enemy->stopTimers();

        // Fondo semitransparente
        QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, width(), height());
        overlay->setBrush(QColor(0, 0, 0, 150));
        overlay->setZValue(10);
        addItem(overlay);

        // Texto de derrota
        QGraphicsTextItem* lostText = new QGraphicsTextItem("¡PERDISTE!");
        lostText->setDefaultTextColor(Qt::red);
        lostText->setFont(QFont("Arial", 36, QFont::Bold));
        lostText->setZValue(11);
        lostText->setPos(width()/2 - lostText->boundingRect().width()/2, height()/2 - 80);
        addItem(lostText);

        // Botón de volver al menú
        backButtonItem = new QGraphicsRectItem(0, 0, 200, 50);
        backButtonItem->setBrush(QColor("#FF4444"));
        backButtonItem->setZValue(12);
        backButtonItem->setPos(width()/2 - 100, height()/2 + 10);
        addItem(backButtonItem);

        QGraphicsTextItem* buttonText = new QGraphicsTextItem("Volver al menú", backButtonItem);
        buttonText->setDefaultTextColor(Qt::white);
        buttonText->setFont(QFont("Arial", 16, QFont::Bold));
        buttonText->setPos(25, 10);
        buttonText->setZValue(13);

        backButtonItem->setAcceptedMouseButtons(Qt::LeftButton);
        backButtonItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

        // Bloquear movimiento de los personajes
        goku->stopMoving();
        enemy->stopMoving();
    }
}
bool GameSceneLevel2::playerFell() const
{
    return goku && goku->y() > height() + 50;
}

void GameSceneLevel2::checkPlatformCollisions()
{
    QRectF gokuFeetRect(
        goku->x() + 15,
        goku->y() + goku->pixmap().height() - 20,
        goku->pixmap().width() - 25,
        20);

    bool gokuOnPlatform = false;

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());
        if (gokuFeetRect.intersects(platformRect) && goku->velocityY > 0) {
            goku->setPos(goku->x(), platformRect.top() - goku->pixmap().height());
            goku->velocityY = 0;
            goku->isJumping = false;
            gokuOnPlatform = true;
            break;
        }
    }

    if (!gokuOnPlatform) {
        QRectF groundRect(0, 560, 800, 40);
        if (gokuFeetRect.intersects(groundRect) && goku->velocityY > 0) {
            goku->setPos(goku->x(), groundRect.top() - goku->pixmap().height());
            goku->velocityY = 0;
            goku->isJumping = false;
        }
    }

    QRectF enemyFeetRect(
        enemy->x() + 5,
        enemy->y() + enemy->pixmap().height() - 10,
        enemy->pixmap().width() - 10,
        12);

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
    if (countdownTimer) countdownTimer->stop();
    gameState = GAME_OVER;
    gameTimer->stop();
    rainTimer->stop();
    if (enemy) enemy->stopTimers();

    QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, width(), height());
    overlay->setBrush(QColor(0, 0, 0, 150));
    overlay->setZValue(10);
    addItem(overlay);

    QGraphicsTextItem* lostText = new QGraphicsTextItem("¡PERDISTES!");
    lostText->setDefaultTextColor(Qt::red);
    lostText->setFont(QFont("Arial", 36, QFont::Bold));
    lostText->setZValue(11);
    lostText->setPos(width()/2 - lostText->boundingRect().width()/2, height()/2 - 80);
    addItem(lostText);

    backButtonItem = new QGraphicsRectItem(0, 0, 200, 50);
    backButtonItem->setBrush(QColor("#FF4444"));
    backButtonItem->setZValue(12);
    backButtonItem->setPos(width()/2 - 100, height()/2 + 10);
    addItem(backButtonItem);

    QGraphicsTextItem* buttonText = new QGraphicsTextItem("Volver al menú", backButtonItem);
    buttonText->setDefaultTextColor(Qt::white);
    buttonText->setFont(QFont("Arial", 16, QFont::Bold));
    buttonText->setPos(25, 10);
    buttonText->setZValue(13);

    backButtonItem->setAcceptedMouseButtons(Qt::LeftButton);
    backButtonItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void GameSceneLevel2::showVictory()
{
    if (countdownTimer) countdownTimer->stop();
    gameState = LEVEL_COMPLETED;
    gameTimer->stop();
    rainTimer->stop();
    if (enemy) enemy->stopTimers();

    QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, width(), height());
    overlay->setBrush(QColor(0, 0, 0, 150));
    overlay->setZValue(10);
    addItem(overlay);

    QGraphicsTextItem* victoryText = new QGraphicsTextItem("¡Nivel 2 Completado!");
    victoryText->setDefaultTextColor(Qt::white);
    victoryText->setFont(QFont("Arial", 36, QFont::Bold));
    victoryText->setZValue(11);
    victoryText->setPos(width()/2 - victoryText->boundingRect().width()/2, height()/2 - 80);
    addItem(victoryText);

    backButtonItem = new QGraphicsRectItem(0, 0, 200, 50);
    backButtonItem->setBrush(QColor("#FFA500"));
    backButtonItem->setZValue(12);
    backButtonItem->setPos(width()/2 - 100, height()/2 + 10);
    addItem(backButtonItem);

    QGraphicsTextItem* buttonText = new QGraphicsTextItem("Volver al menú", backButtonItem);
    buttonText->setDefaultTextColor(Qt::white);
    buttonText->setFont(QFont("Arial", 16, QFont::Bold));
    buttonText->setPos(25, 10);
    buttonText->setZValue(13);

    backButtonItem->setAcceptedMouseButtons(Qt::LeftButton);
    backButtonItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void GameSceneLevel2::keyPressEvent(QKeyEvent *event)
{
    if (gameState != PLAYING) return;

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
    case Qt::Key_G:
        if (Goku2* g2 = dynamic_cast<Goku2*>(goku)) {
            g2->launchKamehameha();
        }
        break;
    }
}

void GameSceneLevel2::keyReleaseEvent(QKeyEvent *event)
{
    if (gameState != PLAYING) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D)
        goku->stopMoving();
}

void GameSceneLevel2::spawnStoneRain()
{
    int x = QRandomGenerator::global()->bounded(50, 750);
    StoneAttack *rainStone = new StoneAttack(StoneAttack::PICCOLO_ATTACK, false);
    rainStone->setPixmap(QPixmap("C:/Users/IVAN/Downloads/rain.png").scaled(30, 30));
    rainStone->setPos(x, 0);
    rainStone->setZValue(5);
    addItem(rainStone);
}

void GameSceneLevel2::setupHealthBars()
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

    enemyHealthBackground = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    enemyHealthBackground->setPos(width() - barWidth - margin, margin);
    enemyHealthBackground->setBrush(Qt::gray);
    enemyHealthBackground->setPen(QPen(Qt::black, 2));
    enemyHealthBackground->setZValue(100);
    addItem(enemyHealthBackground);

    enemyHealthBar = new QGraphicsRectItem(0, 0, barWidth, barHeight);
    enemyHealthBar->setPos(width() - barWidth - margin, margin);
    enemyHealthBar->setBrush(Qt::red);
    enemyHealthBar->setPen(Qt::NoPen);
    enemyHealthBar->setZValue(101);
    addItem(enemyHealthBar);
}

void GameSceneLevel2::updateHealthBars()
{
    if (goku) {
        qreal gokuHealthPercent = static_cast<qreal>(goku->getHealth()) / goku->getMaxHealth();
        gokuHealthBar->setRect(0, 0, 200 * gokuHealthPercent, 20);
    }

    if (enemy) {
        qreal enemyHealthPercent = static_cast<qreal>(enemy->getHealth()) / enemy->getMaxHealth();
        enemyHealthBar->setRect(0, 0, 200 * enemyHealthPercent, 20);
    }
}

void GameSceneLevel2::returnToMenu()
{
    emit goToMainMenu();
}

void GameSceneLevel2::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (gameState != LEVEL_COMPLETED && gameState != GAME_OVER) return;

    if (backButtonItem &&
        backButtonItem->contains(backButtonItem->mapFromScene(event->scenePos()))) {
        emit returnToMenuRequested();
    }

    QGraphicsScene::mousePressEvent(event);
}
