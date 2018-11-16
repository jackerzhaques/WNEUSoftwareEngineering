#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist>
#include <QMediaPlayer>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QList>
#include <QRect>
#include <QPointF>

#include "camera.h"
#include "spritesheet.h"
#include "entity.h"
#include "terrain.h"

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
    void UpdateSprite(QPixmap Sprite);
    void MoveEntity();

private:
    Ui::MainWindow *ui;

    enum ViewChooserPages{
        MainMenu,
        Game
    };

    QGraphicsScene *GraphicsScene = nullptr;

    QMediaPlaylist *BackgroundMusic;
    QMediaPlayer *MusicPlayer;

    Camera *camera = nullptr;
    Entity *entity;
};

#endif // MAINWINDOW_H
