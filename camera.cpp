#include "camera.h"

Camera::Camera(QObject *parent, QGraphicsView *GraphicsViewPointer) : QObject(parent)
{
    this->PanTimer = new QTimer(this);
    connect(this->PanTimer, SIGNAL(timeout()), this, SLOT(UpdatePan()));
    this->GraphicsView = GraphicsViewPointer;

    //Calculate the default view rect starting at 0,0
    QRectF ViewRect(0,0,CurrentSceneWidth, CurrentSceneHeight);

    //Align the graphics view to the top left, and assign the default view rect
    this->GraphicsView->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    this->GraphicsView->setSceneRect(ViewRect);

    this->SetCurrentView(ViewRect);
}

void Camera::FollowEntity(Entity *entity)
{
    //Have this camera center itself on the entity's position every time it moves
    connect(entity, SIGNAL(NewPositionAvailable(QPointF)), this, SLOT(CenterOnEntity()));

    //Save the entity to be used for resuming/stopping
    this->FollowingEntity = entity;

    //Update the current view
    this->CenterOnEntity();
}

void Camera::StopFollowingCurrentEntity()
{
    //Disconnect the signal that accepts new positions
    disconnect(this->FollowingEntity, SIGNAL(NewPositionAvailable(QPointF)), this, SLOT(CenterOnEntity()));
}

void Camera::ResumeFollowingCurrentEntity()
{
    //Reset following the saved entity
    this->FollowEntity(this->FollowingEntity);
}

void Camera::UpdateView()
{
    QRectF CurrentView = this->CurrentViewRect;

    double WidthScale = this->GraphicsView->sceneRect().width() / CurrentView.width();
    double HeightScale = this->GraphicsView->sceneRect().height() / CurrentView.height();
    this->GraphicsView->scale(WidthScale, HeightScale);

    this->GraphicsView->setSceneRect(CurrentView);
}

void Camera::SetCurrentView(QRectF ViewRect)
{
    this->CurrentViewRect = ViewRect;
    this->UpdateView();
}

void Camera::SetCurrentView(QPointF Position){
    //Move the camera's scene to this position
    QRectF NewViewRect(Position.x(), Position.y(), CurrentViewRect.width(), CurrentViewRect.height());

    this->SetCurrentView(NewViewRect);
}

void Camera::CenterOnPoint(QPointF Position)
{
    QRectF ViewRect;
    ViewRect.setX(Position.x() - CurrentSceneWidth/2);
    ViewRect.setY(Position.y() - CurrentSceneHeight/2);
    ViewRect.setWidth(this->CurrentViewRect.width());
    ViewRect.setHeight(this->CurrentViewRect.height());

    this->SetCurrentView(ViewRect);
}

void Camera::MoveToView(QRectF TargetViewRect)
{
    //Update the target view rect to have the width and height of our graphics view
    //Currently zooming is not implemented, so we have to do this to ensure all content
    //will fit within the rect. The rect simply adds a top left corner to view from
    TargetViewRect.setWidth(CurrentSceneWidth);
    TargetViewRect.setHeight(CurrentSceneHeight);

    this->TargetViewRect = TargetViewRect;

    //Calculate the distance between the current view and the target view
    //This will be used to calculate the pan speed of the view
    this->UpdateDistanceToTarget();
    this->UpdateSpeed();

    this->StartPan();
}

void Camera::CenterTo(QPointF TargetViewCenter)
{
    double TargetTopLeft = TargetViewCenter.x() - CurrentSceneWidth/2;
    double TargetTopRight = TargetViewCenter.y() - CurrentSceneHeight/2;
    QRectF TargetRect(TargetTopLeft,TargetTopRight,CurrentSceneWidth,CurrentSceneHeight);

    this->MoveToView(TargetRect);
}

void Camera::CenterOnEntity()
{
    this->CenterOnPoint(this->FollowingEntity->GetCenter());
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

    this->CurrentViewRect = QRectF(x, y, CurrentSceneWidth, CurrentSceneHeight);
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
        this->UpdateView();
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
