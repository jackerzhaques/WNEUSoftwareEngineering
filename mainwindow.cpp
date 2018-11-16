#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showFullScreen();

    ui->ViewChooser->setCurrentIndex(ViewChooserPages::Game);

    this->BackgroundMusic = new QMediaPlaylist(this);
    this->BackgroundMusic->addMedia(QUrl("qrc:/assets/sounds/music/backgrounds/ObservingTheStar.mp3/assets/ObservingTheStar.mp3"));
    this->BackgroundMusic->setPlaybackMode(QMediaPlaylist::Loop);

    this->MusicPlayer = new QMediaPlayer(this);
    this->MusicPlayer->setPlaylist(this->BackgroundMusic);
    //this->MusicPlayer->play();
    this->MusicPlayer->setVolume(50);

    QGraphicsScene *scene = new QGraphicsScene(this->ui->GraphicsView);
    this->ui->GraphicsView->setScene(scene);

    /*
     * Test entity
     */

    entity = new Entity(scene);
    Spritesheet *sheet = entity->GetSpritesheet();

    /*
     * Spritesheet test
     */

    //Test creating a new sheet and giving it a source
    QPixmap Sprite(":/sprites/assets/elisa.png");
    sheet->SetSpriteSource(Sprite);

    //Test adding an idle sprite
    QList<QRect> RectList;
    RectList.append(QRect(4,4,53,53));
    RectList.append(QRect(61,4,53,53));
    RectList.append(QRect(118,4,53,53));
    sheet->SetSprite(Spritesheet::IDLE, RectList);

    //Test adding an attack sprite
    RectList.clear();
    RectList.append(QRect(5,179,88,61));
    RectList.append(QRect(97,179,88,61));
    RectList.append(QRect(189,179,88,61));
    RectList.append(QRect(5,244,88,61));
    RectList.append(QRect(97,244,88,61));
    RectList.append(QRect(189,244,88,61));
    sheet->SetSprite(Spritesheet::ATTACK, RectList);
    sheet->SetCurrentSpritesheet(Spritesheet::ATTACK);

    /*
     * Test adding a walk sprite
     */
    RectList.clear();
    RectList.append(QRect(5,121,53,53));
    RectList.append(QRect(62,121,53,53));
    RectList.append(QRect(119,121,53,53));
    RectList.append(QRect(176,121,53,53));
    RectList.append(QRect(233,121,53,53));
    RectList.append(QRect(290,121,53,53));
    RectList.append(QRect(347,121,53,53));
    RectList.append(QRect(404,121,53,53));
    sheet->SetSprite(Spritesheet::WALK, RectList);

    /*
     * Test changing the direction
     */
    sheet->SetDirection(Spritesheet::RIGHT);

    entity->SetState(Entity::WALKING);
    entity->SetVelocity(1,-0.2);
    entity->SetAcceleration(-0.001, 0.001);

    //connect(sheet, SIGNAL(SpriteChanged(QPixmap)), this, SLOT(UpdateSprite(QPixmap)));


    /*
     * Test graphics rect items
     */
    QGraphicsRectItem *item = new QGraphicsRectItem();
    item->setRect(QRectF(0,0,100,100));
    scene->addItem(item);

    item = new QGraphicsRectItem();
    item->setRect(QRectF(50,300,20,100));
    scene->addItem(item);

    item = new QGraphicsRectItem();
    item->setRect(QRectF(-200,-200,100,200));
    scene->addItem(item);

    item = new QGraphicsRectItem();
    item->setRect(QRectF(600,600,300,300));
    scene->addItem(item);

    QRectF CameraRect(-100,-100,ui->GraphicsView->width(),ui->GraphicsView->height());

    this->camera = new Camera(this, this->ui->GraphicsView);
    this->camera->MoveToView(QRectF(0,0,0,0));

    QTimer *MoveTimer = new QTimer();
    connect(MoveTimer, SIGNAL(timeout()), this, SLOT(MoveEntity()));
    MoveTimer->start(10);

    Terrain *t = new Terrain(scene);
    t->SetSprite(QPixmap(":/walls/assets/twilight-tiles.png"), QRect(161,97,14,45));
    t->ScaleSprite(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateSprite(QPixmap Sprite)
{
}

void MainWindow::MoveEntity(){
    this->entity->Move();
}
