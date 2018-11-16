#ifndef WALL_H
#define WALL_H

#include "terrain.h"

class Wall : public Terrain
{
public:
    Wall(QGraphicsScene *ScenePtr);
};

#endif // WALL_H
