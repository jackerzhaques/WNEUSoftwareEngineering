#pragma once
#ifndef GAME_H
#define GAME_H

#include <QObject>

#include "scene.h"
#include "camera.h"
#include "spritesheet.h"
#include "entity.h"
#include "wall.h"
#include "doodad.h"
#include "npc.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(Scene *GraphicsScenePtr, Camera *CameraPtr, QObject *parent = nullptr);

    QList<Entity*> GetEntities();

signals:

public slots:
    void UpdateEntities();
    void RemoveEntity(Entity* RemovedEntity);

    void KeyPressed(Qt::Key Key);
    void KeyReleased(Qt::Key Key);

    void SetCamera(Camera * CameraPtr);

private:
    Scene *GraphicsScenePtr = nullptr;
    Camera *camera = nullptr;

    //Entity objects
    Entity *Player = nullptr;
    QList<Entity*> EntityList;

    //Terrain objects

    //Item objects

    void ActionKeyPressed(Qt::Key Key);
    void ActionKeyReleased(Qt::Key Key);

    void LoadBackground();
    void LoadSkeletons();
    void LoadPlayer();
    void LoadTerrain();
};

#endif // GAME_H
