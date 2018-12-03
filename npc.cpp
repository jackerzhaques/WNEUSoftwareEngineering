#include "npc.h"
#include "game.h"

NPC::NPC(QGraphicsScene *ScenePtr, Game *GamePtr, QGraphicsItem *parent) : Entity(ScenePtr, parent)
{
    this->GamePtr = GamePtr;

    //Connect our death signal so that this NPC will update its state when dead
    connect(this, SIGNAL(EntityHasDied(Entity*)), this, SLOT(SetNPCInDeathState()));
}

NPC::NPC(Game *GamePtr, Entity *CopyEntity) : Entity(CopyEntity)
{
    this->GamePtr = GamePtr;
}

void NPC::UpdateAI()
{
    qDebug() << this->CurrentAIState;
    if(this->AcquiredEntity)
        qDebug() << this->AcquiredEntity->pos();
    else{
        qDebug() << "NULL";
    }
    this->SwitchToNextAIState();
    this->UpdateCurrentState();
    this->Move();
    qDebug() << "AI";
}

void NPC::SetAIState(NPC::AIState State)
{
    qDebug() << "state";
    switch(State){
        case AIState::IDLING:
            this->SetInIdleState();
            break;
        case AIState::ACQUIRING_TARGET:
            this->SetInAcquireState();
            break;
        case AIState::MOVING_TO_TARGET:
            this->SetInMoveState();
            break;
        case AIState::ATTACKING_TARGET:
            this->SetInAttackState();
            break;
        case AIState::RETURNING:
            this->SetInReturningState();
            break;
        case AIState::DEAD:
            this->SetInDeadState();
            break;
    }
    qDebug() << "set";
}

void NPC::SetAttackRange(double AttackRange)
{
    this->AttackRange = AttackRange;
}

void NPC::SetNPCInDeathState()
{
    qDebug() << "ded";
    //Set in a death state
    this->SetInDeadState();
}

void NPC::UpdateCurrentState()
{
    qDebug() << "stateupdate";
    if(this->CurrentAIState == AIState::IDLING){
        //Do nothing
    }
    else if(this->CurrentAIState == AIState::ACQUIRING_TARGET){
        //Do nothing
    }
    else if(this->CurrentAIState == AIState::MOVING_TO_TARGET){
        //Check if the acquired entity is dead
        if(this->AcquiredEntity->CheckDeath()){
            //The entity has died, return
            this->SetAIState(AIState::RETURNING);
        }
        else{
            //Set the velocity in the direction of the target
            if(this->AcquiredEntity->x() > this->x()){
                //Move right
                this->Walk(this->DefaultVelocity);
            }
            else{
                //Move left
                this->Walk(-this->DefaultVelocity);
            }
        }
    }
    else if(this->CurrentAIState == AIState::ATTACKING_TARGET){
        //Attack the target
        this->Attack();
    }
    else if(this->CurrentAIState == AIState::RETURNING){
        //Update our direction in case we were displaced or the direction has changed
        if(this->x() < this->OriginalPosition.x()){
            this->Walk(NPC_DEFAULT_VELOCITY);
        }
        else{
            this->Walk(-NPC_DEFAULT_VELOCITY);
        }
    }
    else if(this->CurrentAIState == AIState::DEAD){
        //Do nothing
    }
    else{
        //Invalid AI state
        //Do nothing
    }

    //If at all the entity has died, set in a death state
    if(this->CheckDeath()){
        this->SetInDeadState();
    }
    qDebug() << "stateupdatedone";
}

void NPC::SetInIdleState()
{
    //Set this entity in an idling state
    this->SetState(EntityState::IDLING);

    //Set this NPC in idle state
    this->CurrentAIState = AIState::IDLING;

    //Default our acquired entity to null, since there is none at this point in time
    this->AcquiredEntity = nullptr;
}

void NPC::SetInAcquireState()
{
    //Ensure this entity is an idling state
    this->SetState(EntityState::IDLING);

    //Set the AI state
    this->CurrentAIState = AIState::ACQUIRING_TARGET;

    //Find the closest entity
    Entity *ClosestEntity = this->GetClosestEntity();

    //If there isn't an entity close enough, return to idle state
    if(ClosestEntity == nullptr){
        this->SetInIdleState();
    }
    else{
        //An entity has been acquired within range, store this entity
        this->AcquiredEntity = ClosestEntity;

        //Store our current position as the original position this entity was in
        // before leaving to attack an entity.
        this->OriginalPosition = this->pos();
    }
}

void NPC::SetInMoveState()
{
    //Set this entity into a walking state
    this->SetState(EntityState::WALKING);

    //Set the AI state
    this->CurrentAIState = AIState::MOVING_TO_TARGET;
}

void NPC::SetInAttackState()
{
    //We do not need to set the entity state because it will be handled automatically during updates

    //Set the AI state
    this->CurrentAIState = AIState::ATTACKING_TARGET;

    //Clear our velocity from the move state
    this->StopWalking();

    //Face the direction of our target
    if(this->AcquiredEntity->x() > this->x()){
        //target is to our right.
        this->SetDirection(Spritesheet::RIGHT);
    }
    else{
        //Target is to our left.
        this->SetDirection(Spritesheet::LEFT);
    }
}

void NPC::SetInReturningState()
{
    //Set this entity into a walking state
    this->SetState(EntityState::WALKING);

    //Set the AI state
    this->CurrentAIState = AIState::RETURNING;

    //Clear out acquired target
    this->AcquiredEntity = nullptr;

    //Set the velocity in the direction of our origin
    if(this->OriginalPosition.x() > this->x()){
        //Return to our right
        this->Walk(this->DefaultVelocity);
    }
    else{
        //Return to our left
        this->Walk(-this->DefaultVelocity);
    }
}

void NPC::SetInDeadState()
{
    //Set the ai state to dead and wait to be deleted
    this->CurrentAIState = AIState::DEAD;

    //Stop moving
    this->StopWalking();

    //Set the current state to dead
    this->SetState(EntityState::DEAD);

}

void NPC::SwitchToNextAIState()
{
    //Go throguh each possible state and move onto the next state if necessary
    if(this->CurrentAIState == AIState::IDLING){
        //Only move to the next state if an entity is within acquisition range
        Entity *ClosestEntity = this->GetClosestEntity();

        //If the closest entity is not a nullptr, then an entity is within range.
        if(ClosestEntity != nullptr){
            //Set in the acquire state
            this->SetInAcquireState();
        }
    }
    else if(this->CurrentAIState == AIState::ACQUIRING_TARGET){
        //If a target has been acquired then continue to the next state
        if(this->AcquiredEntity != nullptr){
            //Set in the move to target state
            this->SetInMoveState();
        }
        else{
            //A target has not yet been acquired, we will not change states
        }
    }
    else if(this->CurrentAIState == AIState::MOVING_TO_TARGET){
        //If we are in range to attack our target we will switch to an attacking state
        //If we are out of our acquisition range or y height, we will return to our starting point
        //If we are out of range of our target, we will move closer.
        double DistanceToTarget = this->DistanceToEntity(this->AcquiredEntity);
        if(this->TooFarFromOrigin() || this->TargetNotOnSameVerticalPlane()){
            //We must return, we are too far from our origin
            this->SetInReturningState();
        }
        else if(DistanceToTarget > this->AttackRange){
            //We are not close enough to the target to attack it, we must move closer
            //We will stay in this state
        }
        else{
            //We are close enough to attack our target
            //Switch into attack state
            this->SetInAttackState();
        }
    }
    else if(this->CurrentAIState == AIState::ATTACKING_TARGET){
        //If we are out of range of our target, return to the moving to target state
        //If our target is dead, then return to origin
        double DistanceToTarget = this->DistanceToEntity(this->AcquiredEntity);
        if(DistanceToTarget > this->AttackRange){
            //Move to the target
            this->SetInMoveState();
        }
        else if(this->AcquiredEntity->CheckDeath()){
            //The entity is dead, return
            this->SetInReturningState();
        }
        else{
            //Continue attacking the target.
        }
    }
    else if(this->CurrentAIState == AIState::RETURNING){
        //We will keep returning to our origin until we are close enough to stop
        if(this->DistanceFromOrigin() < NPC_ORIGIN_DISTANCE_THRESHOLD){
            //Fix this npc onto the origin
            this->setPos(this->OriginalPosition);

            //Stop walking
            this->StopWalking();

            //Set in an idling state
            this->SetInIdleState();
        }
        else{
            //Continue walking until we hit the origin.
        }
    }
    else if(this->CurrentAIState == AIState::DEAD){
        //This NPC is dead, it cannot do anything, therefor we stay in this state
    }
}

/*
 * Gets the closest entity within this NPCs acquisition range
 */
Entity *NPC::GetClosestEntity()
{
    //Grab the list of all entities from the game
    QList<Entity*> GameEntities = this->GamePtr->GetEntities();

    //Loop through each entity and find the one with the closest distance
    //We default the closest distance to greater than the acquisition range. This ensures
    //that no entity will be detected that is further than the acquisition range
    Entity* ClosestEntity = nullptr;
    double ClosestDistance = this->AcquisitionRange;

    for(int i = 0; i < GameEntities.length(); i++){
        Entity* entity = GameEntities[i];
        //This entity will be included in the list, but we cannot include it in our search
        // so we must skip this entity
        if(entity != this){
            //The entity cannot be dead

            if(!entity->CheckDeath()){
                //Check if the entity is on the same vertical plane as this one
                double VerticalDistance = this->GetVerticalDistanceToEntity(entity);

                if(VerticalDistance < NPC_MAX_VERTICAL_DISTANCE_TO_TARGET){
                    double Distance = this->DistanceToEntity(entity);

                    if(Distance < ClosestDistance){
                        ClosestDistance = Distance;
                        ClosestEntity = entity;
                    }
                }
                else{
                    //The NPC is too high for this NPC to attack it, we will ignore it
                }
            }
            else{
                //The entity is dead, we will ignore it.
            }
        }
        else{
            //The entity in question is this entity, we cannot acquire ourselves we will skip this one.
        }
    }

    return ClosestEntity;
}

double NPC::DistanceToEntity(Entity *entity)
{
    double Distance = 0;

    //Only continue if the entity is not null
    if(entity != nullptr){
        //Grab this position and the entity's position
        qDebug() << "cha";
        QPointF ThisPoint = this->mapRectToScene(this->boundingRect()).center();
        qDebug() << "na";
        QPointF EntityPoint = entity->mapRectToScene(entity->boundingRect()).center();
        qDebug() << "ra";

        //Use a QLineF to get the distance between the points
        QLineF LineBetweenObjects;
        LineBetweenObjects.setP1(ThisPoint);
        LineBetweenObjects.setP2(EntityPoint);

        Distance = LineBetweenObjects.length();
    }
    else{
        //The entity is null so the distance is invalid, return -1
        Distance = -1;
    }

    return Distance;
}

double NPC::GetVerticalDistanceToEntity(Entity *entity)
{
    return abs(entity->y() - this->y());
}

double NPC::DistanceFromOrigin()
{
    return abs(this->x() - this->OriginalPosition.x());
}

bool NPC::TooFarFromOrigin()
{
    //If our distance from origin is greater than our acquisition range, then we return true
    if(DistanceFromOrigin() > this->AcquisitionRange){
        return true;
    }
    else{
        return false;
    }
}

/*
 * This function will return true if the target and this npc are not at the same height
 * This function does have some leeway, defined by NPC_MAX_VERTICAL_DISTANCE_TO_TARGET
 */
bool NPC::TargetNotOnSameVerticalPlane()
{
    if(this->GetVerticalDistanceToEntity(this->AcquiredEntity) > NPC_MAX_VERTICAL_DISTANCE_TO_TARGET){
        return true;
    }
    else{
        return false;
    }
}
