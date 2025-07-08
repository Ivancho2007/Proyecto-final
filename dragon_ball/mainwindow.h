#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>


class QStackedWidget;
class QGraphicsView;
class GameScene;
class GameSceneLevel2;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void returnToMenu();

private slots:
    void startLevel1();
    void startLevel2();

private:
    QStackedWidget *stackedWidget;
    QGraphicsView *gameView;
    GameScene *gameScene;
    GameSceneLevel2 *gameSceneLevel2;
    QMediaPlayer *musicPlayer;
    QAudioOutput *audioOutput;


    void setupMenu();
};

#endif // MAINWINDOW_H


