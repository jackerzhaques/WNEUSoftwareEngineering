#include "camera.h"

Camera::Camera(QObject *parent, QGraphicsView *GraphicsViewPointer) : QObject(parent)
{
    this->PanTimer = new QTimer(this);
    connect(this->PanTimer, SIGNAL(timeout()), this, SLOT(UpdatePan()));
    this->GraphicsView = GraphicsViewPointer;

    //Calculate the default view rect starting at 0,0
    QRectF ViewRect(0,0,DEFAULT_SCENE_WIDTH, DEFAULT_SCENE_HEIGHT);
    this->CurrentViewRect = ViewRect;

    //Align the graphics view to the top left, and assign the default view rect
    this->GraphicsView->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    this->GraphicsView->setSceneRect(ViewRect);
}

void Camera::UpdateView()
{
    QRectF CurrentView = this->CurrentViewRect;
    this->GraphicsView->setSceneRect(CurrentView);
}

void Camera::SetCurrentView(QRectF ViewRect)
{
    //Update the rect so its width and height match the graphics view
    //Currently zooming is not implemented, if it is then this will have to be changed to match it
    ViewRect.setWidth(DEFAULT_SCENE_WIDTH);
    ViewRect.setHeight(DEFAULT_SCENE_HEIGHT);

    this->CurrentViewRect = ViewRect;

    this->UpdateView();
}

void Camera::MoveToView(QRectF TargetViewRect)
{
    //Update the target view rect to have the width and height of our graphics view
    //Currently zooming is not implemented, so we have to do this to ensure all content
    //will fit within the rect. The rect simply adds a top left corner to view from
    TargetViewRect.setWidth(DEFAULT_SCENE_WIDTH);
    TargetViewRect.setHeight(DEFAULT_SCENE_HEIGHT);

    this->TargetViewRect = TargetViewRect;

    //Calculate the distance between the current view and the target view
    //This will be used to calculate the pan speed of the view
    this->UpdateDistanceToTarget();

    this->StartPan();
}

void Camera::CenterTo(QPointF TargetViewCenter)
{
    double TargetTopLeft = TargetViewCenter.x() - DEFAULT_SCENE_WIDTH/2;
    double TargetTopRight = TargetViewCenter.y() - DEFAULT_SCENE_HEIGHT/2;
    QRectF TargetRect(TargetTopLeft,TargetTopRight,DEFAULT_SCENE_WIDTH,DEFAULT_SCENE_HEIGHT);

    this->MoveToView(TargetRect);
}

void Camera::StartPan()
{
    //Calculate the starting pan speed
    this->UpdateSpeed();

    //Set the timer and start it
    this->PanTimer->start(10);
}

void Camera::UpdatePan()
{
    //Move the current frame
    double x = this->CurrentViewRect.x() + this->CurrentPanSpeedX;
    double y = this->CurrentViewRect.y() + this->CurrentPanSpeedY;

    this->CurrentViewRect = QRectF(x, y, DEFAULT_SCENE_WIDTH, DEFAULT_SCENE_HEIGHT);
    //Update the view
    this->UpdateView();

    //Refresh distance to target
    this->UpdateDistanceToTarget();

    //Update the speed
    this->UpdateSpeed();

    //If the current view rect and the target view rect are less than 0.5 from each other
    //Set the current view to the target view and stop the pan
    if(abs(this->CurrentXDistanceToTarget) < 2 && abs(this->CurrentYDistanceToTarget) < 2){
        this->PanTimer->stop();
        this->CurrentViewRect = this->TargetViewRect;
        emit(PanFinished());
    }
}

void Camera::UpdateDistanceToTarget()
{
    this->CurrentYDistanceToTarget = this->TargetViewRect.y() - this->CurrentViewRect.y();
    this->CurrentXDistanceToTarget = this->TargetViewRect.x() - this->CurrentViewRect.x();
}

void Camera::UpdateSpeed()
{
    this->CurrentPanSpeedX = this->CurrentXDistanceToTarget/PAN_SPEED_FACTOR;
    this->CurrentPanSpeedY = this->CurrentYDistanceToTarget/PAN_SPEED_FACTOR;
}
