#include "gamescene.h"
#include <QKeyEvent>
#include <QGraphicsRectItem>
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
    // Fondo (se mantiene igual)
    QPixmap fondo("C:/Users/IVAN/Downloads/fondo1.png");
    if (fondo.isNull()) {
        fondo = QPixmap(800, 600);
        fondo.fill(Qt::blue);
    }
    backgroundItem = new QGraphicsPixmapItem(fondo);
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);

    // Goku - posición inicial (abajo a la izquierda)
    goku = new Goku();
    goku->setPos(50, 500 - goku->pixmap().height()); // X: 50, Y: ajustado a la base
    addItem(goku);

    // Piccolo - posición inicial (derecha del escenario)
    enemy = new Enemy();
    enemy->setPos(650, 500 - enemy->pixmap().height()); // X: 650, Y: misma base que Goku
    addItem(enemy);

    // Plataformas - posiciones estratégicas
    QList<QRectF> platformRects = {
        // Formato: QRectF(x, y, ancho, alto)
        QRectF(200, 290, 120, 15),  // Plataforma baja izquierda
        QRectF(450, 380, 100, 15),  // Plataforma central más alta
        QRectF(650, 310, 100, 15)   // Plataforma derecha más alta
    };

    for (const QRectF &rect : platformRects) {
        QGraphicsRectItem *platform = new QGraphicsRectItem(rect);
        platform->setBrush(QColor(0, 200, 0, 200)); // Verde semitransparente
        platform->setPen(QPen(Qt::black, 1));
        platform->setZValue(1); // Encima del fondo
        addItem(platform);
        platforms.append(platform);
    }

    // Ajustar el nivel del suelo para Goku
    goku->groundLevel = 520; // Coincide con la base de la ventana
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

    // Verificar colisiones con plataformas
    QRectF gokuRect = goku->boundingRect().translated(goku->pos());
    bool onPlatform = false;

    for (QGraphicsRectItem* platform : platforms) {
        QRectF platformRect = platform->rect().translated(platform->pos());

        if (gokuRect.intersects(platformRect) &&
            goku->velocityY > 0 &&
            gokuRect.bottom() <= platformRect.top() + 5) {
            goku->setPos(goku->x(), platformRect.top() - gokuRect.height());
            goku->velocityY = 0;
            goku->isJumping = false;
            onPlatform = true;
            break;
        }
    }

    // Si no está en plataforma, verificar suelo base
    if (!onPlatform && goku->y() + goku->pixmap().height() >= goku->groundLevel) {
        goku->setPos(goku->x(), goku->groundLevel - goku->pixmap().height());
        goku->velocityY = 0;
        goku->isJumping = false;
    }
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
