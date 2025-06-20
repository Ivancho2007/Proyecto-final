#include "gamescene.h"
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 800, 600); // Tamaño inicial
    setupLevel();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);
    gameTimer->start(16); // ~60 FPS
}

void GameScene::setupLevel()
{
    // Fondo
    QPixmap fondo("C:/Users/IVAN/Downloads/fondo1.png");
    if (fondo.isNull()) {
        fondo = QPixmap(800, 600);
        fondo.fill(Qt::blue);
    }

    backgroundItem = new QGraphicsPixmapItem(fondo);
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);

    // Goku
    goku = new Goku();
    goku->setPos(100, 405);
    addItem(goku);

    // Enemy (Piccolo)
    enemy = new Enemy();
    enemy->setPos(1200, 560); // posición en el nivel
    addItem(enemy);

    // Plataformas
    QPixmap plataformaPixmap("C:/Users/IVAN/Downloads/PLATAFORMA.png");
    plataformaPixmap = plataformaPixmap.scaled(80, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QList<QPoint> positions = {
        QPoint(600, 500),
        QPoint(800, 450),
        QPoint(1000, 400)
    };


    for (const QPoint &pos : positions) {
        QGraphicsPixmapItem *platform = new QGraphicsPixmapItem(plataformaPixmap);
        platform->setPos(pos);
        addItem(platform);
        platforms.append(platform);
    }

    resizeBackground();
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
    goku->advance(1);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A: goku->moveLeft(); break;
    case Qt::Key_D: goku->moveRight(); break;
    case Qt::Key_Space:
    case Qt::Key_W: goku->jump(); break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        goku->stopMoving();
    }
}
