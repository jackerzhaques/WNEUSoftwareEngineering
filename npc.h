#ifndef NPC_H
#define NPC_H

#include "entity.h"
#include <QLineF>

#define NPC_DEFAULT_ACQUISITION_RANGE           300
#define DEFAULT_NPC_ATTACK_RANGE                70
#define NPC_DEFAULT_VELOCITY                    15
#define NPC_MAX_VERTICAL_DISTANCE_TO_TARGET     30
#define NPC_ORIGIN_DISTANCE_THRESHOLD           25

//Forward declare the game class
class Game;

class NPC : public Entity
{
public:
    NPC(QGraphicsScene *ScenePtr, Game *GamePtr, QGraphicsItem *Parent);
    NPC(Game *GamePtr, Entity *CopyEntity);

    //Updates all AI and movement for this NPC
    void UpdateAI();

    /*
     * NPC AI Has the following states
     *
     * IDLING: The NPC is doing nothing. It will actively scan for targets
     *
     * ACQUIRING_TARGET: A target has been found in range. The
     *                      NPC will set all variables needed then
     *                      move to the MOVING_TO_TARGET STATE
     *
     * MOVING_TO_TARGET: The NPC is on route to the target, it will continuously move
     *                      in the targets direction until the target is out of range
     *                      or the target is close enough to attack and switch to
     *                      ATTACKING_TARGET state.
     *
     * ATTACKING_TARGET: The NPC is in range of a target, and will continue to attack
     *                      the target until the target dies, this NPC dies, or the target
     *                      is out of range. If the target is out of range, the NPC
     *                      will switch to MOVING_TO_TARGET state.
     *
     * RETURNING:        The NPC has either killed its target, or the target has left
     *                      its acquisition range. The NPC will return to its original position
     */
    enum AIState{
        IDLING,
        ACQUIRING_TARGET,
        MOVING_TO_TARGET,
        ATTACKING_TARGET,
        RETURNING,
        DEAD
    };

    void SetAIState(AIState State);

    void SetAttackRange(double AttackRange);

public slots:
    void SetNPCInDeathState();

private:
    //External pointers
    Game *GamePtr;

    AIState CurrentAIState = IDLING;
    void UpdateCurrentState();
    void SetInIdleState();
    void SetInAcquireState();
    void SetInMoveState();
    void SetInAttackState();
    void SetInReturningState();
    void SetInDeadState();
    void SwitchToNextAIState();

    //AI Variables
    double AcquisitionRange = NPC_DEFAULT_ACQUISITION_RANGE;
    QPointF OriginalPosition;
    Entity *AcquiredEntity = nullptr;
    double AttackRange = DEFAULT_NPC_ATTACK_RANGE;
    double DefaultVelocity = NPC_DEFAULT_VELOCITY;

    //Entity acquisition functions
    Entity *GetClosestEntity();
    double DistanceToEntity(Entity* entity);
    double GetVerticalDistanceToEntity(Entity* entity);

    //Returning functions
    double DistanceFromOrigin();
    bool TooFarFromOrigin();
    bool TargetNotOnSameVerticalPlane();

};

#endif // NPC_H
