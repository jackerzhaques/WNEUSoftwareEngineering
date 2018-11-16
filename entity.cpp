#include "entity.h"

Entity::Entity(QGraphicsScene *ScenePtr, QObject *parent) : QObject(parent)
{
    this->Sprites = new Spritesheet();
    this->Graphic = new QGraphicsPixmapItem();
    this->ScenePtr = ScenePtr;

    //Add our QGraphicsPixmapItem to the scene
    this->ScenePtr->addItem(Graphic);

    //Connect the signals from our spritesheet
    connect(this->Sprites, SIGNAL(StateChangedToIdle()), this, SLOT(ChangeStateToIdle()));
    connect(this->Sprites, SIGNAL(SpriteChanged(QPixmap)), this, SLOT(UpdateCurrentSprite(QPixmap)));
}

/*
 * Incrementally moves this entity one step
 *
 * This takes into consideration acceleration and velocity
 */
void Entity::Move()
{
    this->xVel += this->xAccel;
    this->yVel += this->yAccel;

    double X = this->Graphic->x() + this->xVel;
    double Y = this->Graphic->y() + this->yVel;

    this->SetPosition(QPointF(X,Y));
}

/*
 * Immediately moves this entity to the point specified
 */
void Entity::SetPosition(QPointF Pos)
{
    this->Graphic->setPos(Pos);
}

/*
 * Deals damage to this entity, returning the amount dealt
 *
 * This will generate a floating damage text above this entity
 */
double Entity::TakeDamage(double Damage)
{
    //Calculate the damage after defense
    Damage -= this->Defense;

    //Clamp the damage at 0
    if(Damage < 0) Damage = 0;

    //Subtract from current health
    this->Health -= Damage;

    //If the entity died, set in a death state
    if(this->CheckDeath()){
        this->SetState(DEAD);
    }

    this->GenerateDamageText(Damage);

    //Return the damage dealt
    return Damage;
}

/*
 * Damages a target entity, with the source being this entity
 *
 * Returns the amount of damage dealt
 */
double Entity::DamageTarget(Entity *Target, double Damage)
{
    return Target->TakeDamage(Damage);
}

/*
 * Heals this entity for the specified amount
 * Returns the amount healed
 */

double Entity::Heal(double HealAmount)
{
    //Increment this entity's health by the amount specified
    this->Health += HealAmount;

    //Clamp the health at the max health value
    if(this->Health > this->MaxHealth){
        this->Health = this->MaxHealth;
    }

    return HealAmount;
}

/*
 * Returns true if this entity is dead
 * Returns false otherwise
 */
bool Entity::CheckDeath()
{
    //If this entity's health is less than 0, this entity is dead
    if(this->Health <= 0){
        return true;
    }
    else return false;
}

/*
 * Sets the state of this entity as well as the spritesheet contained
 */
void Entity::SetState(Entity::EntityState State)
{
    this->CurrentState = State;

    //Update our Spritesheet too
    switch(State){
        case IDLING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::IDLE);
            break;
        case WALKING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::WALK);
            break;
        case ATTACKING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::ATTACK);
            break;
        case SHOOTING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::SHOOT);
            break;
        case DEAD:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::DEATH);
            break;
        case CROUCHING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::CROUCH);
            break;
        case JUMPING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::JUMP);
            break;
        case FALLING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::FALL);
            break;
        case CHANNELING:
            this->Sprites->SetCurrentSpritesheet(Spritesheet::CHANNEL);
            break;
    }
}

/*
 * Returns the private spritesheet in this object
 */
Spritesheet* Entity::GetSpritesheet()
{
    return this->Sprites;
}

/*
 *Updates the internal spritesheet with the new pointer
 */
void Entity::UpdateSpritesheet(Spritesheet *Sprite)
{
    this->Sprites = Sprite;
}

void Entity::SetAcceleration(double i, double j)
{
    this->xAccel = i;
    this->yAccel = j;
}

void Entity::SetVelocity(double i, double j)
{
    this->xVel = i;
    this->yVel = j;
}

/*
 * Generates floating damage text above the entity
 */
void Entity::GenerateDamageText(double Damage)
{
    //Generate damage text
}

/*
 * SLOT
 *
 * This slot is called externally when a dependent module has changed its state to idling (Spritesheet)
 */
void Entity::ChangeStateToIdle()
{
    //Change the state to idling, but do not update the spritesheet or else the first frame will play twice.
    this->CurrentState = IDLING;
}

/*
 * SLOT
 *
 * This slot is called externally when the dependent spritesheet has updated its frame.
 * This slot will update the internal frame with it
 */
void Entity::UpdateCurrentSprite(QPixmap NewSprite)
{
    this->Graphic->setPixmap(NewSprite);
}
