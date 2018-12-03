#include "wall.h"

Wall::Wall(QGraphicsScene *ScenePtr) : Terrain(ScenePtr)
{
    this->setZValue(2);
}
