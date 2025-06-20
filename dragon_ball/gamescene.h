#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem> // Cambiado de QGraphicsPixmapItem
#include <QTimer>
#include "goku.h"
#include "enemy.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    void resizeBackground();

    const QList<QGraphicsRectItem*>& getPlatforms() const { return platforms; } // Cambiado a RectItem

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void update();

private:
    void setupLevel();

    Goku *goku;
    Enemy *enemy;
    QGraphicsPixmapItem *backgroundItem;
    QList<QGraphicsRectItem*> platforms; // Cambiado a RectItem
    QTimer *gameTimer;
};

#endif // GAMESCENE_H

