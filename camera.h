/*
 * Class:   Camera
 * Date:    11/4/2018
 * Description:
 *  The camera class controls where the scene is centered on, as well as
 *  panning and zooming the view.
 *
 *  For all purposes, what the camera sees is what the player sees.
 *
 * Revision History
 * *************************************************************************
 * **                           Revision History                          **
 * *************************************************************************
 * **   Description                     Author                  Date      **
 * **   Initial revision                Todd Morehouse          11/4/2018 **
 * **   x                               x                       x         **
 * **   x                               x                       x         **
 * *************************************************************************
*/

#ifndef CAMERA_H
#define CAMERA_H

#define PAN_SPEED_FACTOR        50
#define DEFAULT_SCENE_WIDTH     1000
#define DEFAULT_SCENE_HEIGHT    900

#include <QObject>
#include <QRectF>
#include <QPointF>
#include <QTimer>
#include <QGraphicsView>
#include <QDebug>
#include <QMatrix>

#include "entity.h"

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr, QGraphicsView *GraphicsViewPointer = nullptr);
    void FollowEntity(Entity *entity);
    void StopFollowingCurrentEntity();
    void ResumeFollowingCurrentEntity();
    QRectF CurrentViewRect;
    QRectF TargetViewRect;

signals:
    void PanFinished();

public slots:
    void UpdateView();
    void SetCurrentView(QRectF ViewRect);
    void SetCurrentView(QPointF Position);
    void CenterOnPoint(QPointF Position);
    void MoveToView(QRectF TargetViewRect);
    void CenterTo(QPointF TargetViewCenter);
    void CenterOnEntity();

private slots:
    void StartPan();
    void UpdatePan();

private:
    QTimer *PanTimer = nullptr;

    void UpdateDistanceToTarget();
    void UpdateSpeed();
    QGraphicsView *GraphicsView;

    double CurrentXDistanceToTarget = 0;
    double CurrentYDistanceToTarget = 0;
    double CurrentPanSpeedX = 0;
    double CurrentPanSpeedY = 0;

    Entity *FollowingEntity = nullptr;

    int CurrentSceneWidth = DEFAULT_SCENE_WIDTH;
    int CurrentSceneHeight = DEFAULT_SCENE_HEIGHT;
};

#endif // CAMERA_H
