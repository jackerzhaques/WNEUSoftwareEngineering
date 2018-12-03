#include "doodad.h"

Doodad::Doodad(QGraphicsScene *ScenePtr) : Terrain(ScenePtr)
{
    this->setZValue(0);
}
