#include "mainwindow.h"
#include "gamescene.h"
#include "gamescenelevel2.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStackedLayout>



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
    // Música de fondo
    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(audioOutput);

    // Reemplaza la ruta por tu archivo real .mp3
    musicPlayer->setSource(QUrl::fromLocalFile("C:/Users/IVAN/Downloads/music.mp3"));
    audioOutput->setVolume(50); // volumen del 0 al 100
    musicPlayer->setLoops(QMediaPlayer::Infinite); // repetir sin fin
    musicPlayer->play();

}

void MainWindow::setupMenu()
{
    QWidget *menuWidget = new QWidget();
    menuWidget->setFixedSize(800, 600);


    QLabel *backgroundLabel = new QLabel(menuWidget);
    backgroundLabel->setPixmap(QPixmap("C:/Users/IVAN/Downloads/fondoMenu.png").scaled(800, 600, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, 800, 600);
    backgroundLabel->lower();


    QLabel *title = new QLabel("DragonBall Game", menuWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 36px; color: yellow;");
    title->setGeometry(0, 100, 800, 50);
    title->raise();  // Asegura que esté encima

    QPushButton *startLevel1 = new QPushButton("BUSQUEDA", menuWidget);
    QPushButton *startLevel2 = new QPushButton("BATALLA FINAL", menuWidget);

    startLevel1->setStyleSheet("font-size: 24px; padding: 10px; background-color: #1E90FF; color: white;");
    startLevel2->setStyleSheet("font-size: 24px; padding: 10px; background-color: #32CD32; color: white;");

    startLevel1->setGeometry(300, 200, 200, 50);
    startLevel2->setGeometry(300, 270, 200, 50);

    startLevel1->raise();
    startLevel2->raise();

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
    if (gameView) {
        stackedWidget->removeWidget(gameView);
        delete gameView;
        gameView = nullptr;
    }

    if (gameScene) {
        gameScene->deleteLater();
        gameScene = nullptr;
    }

    if (gameSceneLevel2) {
        gameSceneLevel2->deleteLater();
        gameSceneLevel2 = nullptr;
    }

    stackedWidget->setCurrentIndex(0);
}


MainWindow::~MainWindow()
{

}
