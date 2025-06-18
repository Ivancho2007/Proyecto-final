#include "mainwindow.h"
#include "gamescene.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QGraphicsWidget>
#include <QStackedWidget>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , gameView(nullptr)
    , gameScene(nullptr)
{
    setCentralWidget(stackedWidget);
    setupMenu();
}

void MainWindow::setupMenu()
{
    QWidget *menuWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(menuWidget);

    QLabel *title = new QLabel("DragonBall Game - Nivel 1");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 36px; color: #FFD700;");

    QPushButton *startButton = new QPushButton("Iniciar Juego");
    startButton->setStyleSheet("font-size: 24px; padding: 10px; background-color: #FF4500; color: white;");

    layout->addWidget(title);
    layout->addWidget(startButton);
    layout->setAlignment(Qt::AlignCenter);

    stackedWidget->addWidget(menuWidget);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);
}

void MainWindow::startGame()
{
    if (gameScene) delete gameScene;
    if (gameView) delete gameView;

    gameScene = new GameScene(this);
    gameView = new QGraphicsView(gameScene);

    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFrameStyle(QFrame::NoFrame);
    gameView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    gameView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    stackedWidget->addWidget(gameView);
    stackedWidget->setCurrentWidget(gameView);
    gameView->setFocus();

    // Ajustar tamaño inicial
    QSize size = this->size();
    gameView->setFixedSize(size);
    gameScene->setSceneRect(0, 0, size.width(), size.height());
    gameScene->resizeBackground();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (gameScene && gameView) {
        QSize size = event->size();
        gameView->setFixedSize(size);
        gameScene->setSceneRect(0, 0, size.width(), size.height());
        gameScene->resizeBackground();
    }
}

MainWindow::~MainWindow()
{
    // Limpieza automática por herencia de padres
}
