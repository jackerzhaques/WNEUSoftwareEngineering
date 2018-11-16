#ifndef DESTRUCTIBLE_H
#define DESTRUCTIBLE_H

#include "terrain.h"


class Destructible : public Terrain
{
public:
    Destructible(QGraphicsScene *ScenePtr);
};

#endif // DESTRUCTIBLE_H
