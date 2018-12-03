#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist>
#include <QMediaPlayer>

#include <QGraphicsView>
#include <QList>
#include <QRect>
#include <QPointF>
#include <QGuiApplication>
#include <QScreen>
#include <QMatrix>

#include "scene.h"
#include "game.h"

#define DEFAULT_WINDOW_HEIGHT   1000
#define DEFAULT_WINDOW_WIDTH    1000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    Ui::MainWindow *ui;

    void ScaleToUsersDevice();
    double WindowScalingFactor = 1;

    enum ViewChooserPages{
        MAIN_MENU,
        GAME
    };

    Scene *GraphicsScene = nullptr;

    QMediaPlaylist *BackgroundMusic;
    QMediaPlayer *MusicPlayer;

    Camera *camera = nullptr;

    Game *game = nullptr;
};

#endif // MAINWINDOW_H
