#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();

    //Scale this window to the users device
    this->ScaleToUsersDevice();

    //Create a new scene and add it to our graphics view
    this->GraphicsScene = new Scene(this);
    this->ui->GraphicsView->setScene(GraphicsScene);

    //Create a new camera object
    this->camera = new Camera(this, this->ui->GraphicsView);

    //Create a new game object
    this->game = new Game(this->GraphicsScene, this->camera, this);

    //Test: Set the view to show the game by default
    this->ui->ViewChooser->setCurrentIndex(GAME);

    this->ui->GraphicsView->setBackgroundBrush(QBrush(Qt::black));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ScaleToUsersDevice()
{
    //Set the QGraphicsView size to the default size
    /*
     * WARNING: If this is not done, then the graphics view will report a default size of 30 x 100, and the calculations will fail!
     */
    this->ui->GraphicsView->resize(DEFAULT_SCENE_WIDTH,DEFAULT_SCENE_HEIGHT);

    //Get the users screen size
    QRect ScreenSize = QGuiApplication::primaryScreen()->availableGeometry();

    //Calculate the height of the screen as 75%
    double WindowHeight = ScreenSize.height() * 0.75;

    //Calculate the scaling factor between the original size.
    this->WindowScalingFactor = WindowHeight/DEFAULT_WINDOW_HEIGHT;

    //Calculate the new width
    double WindowWidth = DEFAULT_WINDOW_WIDTH * WindowScalingFactor;

    //Apply the calculated size
    this->resize(static_cast<int>(WindowWidth), static_cast<int>(WindowHeight));

    //Center this window on the screen
    this->move(ScreenSize.center() - this->rect().center());

    //Scale the QGraphicsView.
    ui->GraphicsView->scale(this->WindowScalingFactor, this->WindowScalingFactor);
}
