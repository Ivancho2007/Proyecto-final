#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QGraphicsView;
class GameScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void startGame();

private:
    QStackedWidget *stackedWidget;
    QGraphicsView *gameView;
    GameScene *gameScene;

    void setupMenu();
};

#endif // MAINWINDOW_H
