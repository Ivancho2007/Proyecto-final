#include "mainwindow.h"
#include "gamescene.h"
#include "gamescenelevel2.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , gameView(nullptr)
    , gameScene(nullptr)
    , gameSceneLevel2(nullptr)
{
    setFixedSize(800, 600);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setCentralWidget(stackedWidget);
    setupMenu();
}

void MainWindow::setupMenu()
{
    QWidget *menuWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(menuWidget);

    QLabel *title = new QLabel("DragonBall Game");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 36px; color: #FFD700;");

    QPushButton *startLevel1 = new QPushButton("Nivel 1");
    QPushButton *startLevel2 = new QPushButton("Nivel 2");

    startLevel1->setStyleSheet("font-size: 24px; padding: 10px; background-color: #1E90FF; color: white;");
    startLevel2->setStyleSheet("font-size: 24px; padding: 10px; background-color: #32CD32; color: white;");

    layout->addWidget(title);
    layout->addWidget(startLevel1);
    layout->addWidget(startLevel2);
    layout->setAlignment(Qt::AlignCenter);

    stackedWidget->addWidget(menuWidget);

    connect(startLevel1, &QPushButton::clicked, this, &MainWindow::startLevel1);
    connect(startLevel2, &QPushButton::clicked, this, &MainWindow::startLevel2);
}

void MainWindow::startLevel1()
{
    if (gameScene) delete gameScene;
    if (gameView) delete gameView;

    gameScene = new GameScene(this);
    connect(gameScene, &GameScene::returnToMenuRequested, this, &MainWindow::returnToMenu);

    gameView = new QGraphicsView(gameScene);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFrameStyle(QFrame::NoFrame);
    gameView->setFixedSize(800, 600);

    stackedWidget->addWidget(gameView);
    stackedWidget->setCurrentWidget(gameView);
    gameView->setFocus();

    gameScene->setSceneRect(0, 0, 800, 600);
    gameScene->resizeBackground();
}

void MainWindow::startLevel2()
{
    if (gameSceneLevel2) delete gameSceneLevel2;
    if (gameView) delete gameView;

    gameSceneLevel2 = new GameSceneLevel2(this);
    connect(gameSceneLevel2, &GameSceneLevel2::returnToMenuRequested, this, &MainWindow::returnToMenu);

    gameView = new QGraphicsView(gameSceneLevel2);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFrameStyle(QFrame::NoFrame);
    gameView->setFixedSize(800, 600);

    stackedWidget->addWidget(gameView);
    stackedWidget->setCurrentWidget(gameView);
    gameView->setFocus();

    gameSceneLevel2->setSceneRect(0, 0, 800, 600);
    gameSceneLevel2->resizeBackground();
}

void MainWindow::returnToMenu()
{
    if (gameScene) {
        stackedWidget->removeWidget(gameView);
        delete gameScene;
        gameScene = nullptr;
    }
    if (gameSceneLevel2) {
        stackedWidget->removeWidget(gameView);
        delete gameSceneLevel2;
        gameSceneLevel2 = nullptr;
    }
    if (gameView) {
        delete gameView;
        gameView = nullptr;
    }

    stackedWidget->setCurrentIndex(0); // menú principal
}

MainWindow::~MainWindow()
{
    // Limpieza automática
}

