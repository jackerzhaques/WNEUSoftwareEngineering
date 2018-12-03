#ifndef DOODAD_H
#define DOODAD_H

#include "terrain.h"

/*
 * Doodads are simply terrain without collision
 */

class Doodad : public Terrain
{
public:
    Doodad(QGraphicsScene *ScenePtr);
};

#endif // DOODAD_H
