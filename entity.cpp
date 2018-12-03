#include "entity.h"

Entity::Entity(QGraphicsScene *ScenePtr, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    this->Sprites = new Spritesheet();
    this->ScenePtr = ScenePtr;

    //Add our QGraphicsPixmapItem to the scene
    this->ScenePtr->addItem(this);

    //Set the Z value of this object to be in front of background
    this->setZValue(1);

    //Connect the signals from our spritesheet
    connect(this->Sprites, SIGNAL(StateChangedToIdle()), this, SLOT(ChangeStateToIdle()));
    connect(this->Sprites, SIGNAL(SpriteChanged(QPixmap)), this, SLOT(UpdateCurrentSprite(QPixmap)));

    //Initialize our collision detection rectangle
    this->CollisionRect = new QGraphicsRectItem(this);
    this->CollisionRect->setRect(this->mapRectToScene(this->boundingRect()));
    this->CollisionRect->hide();

    //Initialize our attack collision detection rectangle
    this->AttackCollisionRect = new QGraphicsRectItem(this);
    this->AttackCollisionRect->hide();
}

Entity::Entity(Entity *CopyEntityPtr)
{
    //Copy the parent, scene pointer, spritesheet, and position over.
    this->setParent(CopyEntityPtr->parent());
    this->ScenePtr = CopyEntityPtr->scene();
    this->ScenePtr->addItem(this);

    Spritesheet *SpriteSheet = new Spritesheet(CopyEntityPtr->GetSpritesheet());
    this->UpdateSpritesheet(SpriteSheet);

    this->setPos(CopyEntityPtr->pos());

    //Copy spritesheet padding
    this->xPadding = CopyEntityPtr->xPadding;
    this->yPadding = CopyEntityPtr->yPadding;

    //Connect the signals from our spritesheet
    connect(this->Sprites, SIGNAL(StateChangedToIdle()), this, SLOT(ChangeStateToIdle()));
    connect(this->Sprites, SIGNAL(SpriteChanged(QPixmap)), this, SLOT(UpdateCurrentSprite(QPixmap)));

    //Initialize our collision detection rectangle
    this->CollisionRect = new QGraphicsRectItem(this);
    this->CollisionRect->setRect(this->mapRectToScene(this->boundingRect()));
    this->CollisionRect->hide();

    //Initialize our attack collision detection rectangle
    this->AttackCollisionRect = new QGraphicsRectItem(this);
    this->AttackCollisionRect->setRect(CopyEntityPtr->AttackCollisionRect->rect());
    this->AttackCollisionRect->hide();

    //Initialize this object with the remaining states
    this->setZValue(1);

    //Default this entity to an idle state
    this->SetState(IDLING);
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

    //Detect collision by sliding a rect along the x and y axis, and checking collision each time.
    //If the rect collides along a certain axis, do not stop the other axis from sliding
    bool HasCollidedAlongX = false;
    bool HasCollidedAlongY = false;

    double xVelIncrement = this->xVel * ENTITY_MOVE_STEP_PERCENTAGE;
    double yVelIncrement = this->yVel * ENTITY_MOVE_STEP_PERCENTAGE;

    double CurrentXOffset = 0;
    double CurrentYOffset = 0;

    int TotalIncrements = static_cast<int>(1.0 / ENTITY_MOVE_STEP_PERCENTAGE);

    //Reset our collision rect to (0,0)
    this->CollisionRect->setPos(0,0);
    double RectX = 0 + this->xPadding;
    double RectY = 0 + this->yPadding;
    double RectWidth = this->boundingRect().width() - 2*this->xPadding;
    double RectHeight = this->boundingRect().height() - 2*this->yPadding;
    this->CollisionRect->setRect(RectX, RectY, RectWidth, RectHeight);

    for(int i = 0; i < TotalIncrements; i++){
        //If we have not detected x collision, move the rect along the x axis
        if(!HasCollidedAlongX){
            CurrentXOffset += xVelIncrement;
            this->CollisionRect->setX(CurrentXOffset);

            if(DetectCollision()){
                HasCollidedAlongX = true;

                CurrentXOffset -= xVelIncrement;
                this->CollisionRect->setX(CurrentXOffset);
            }
        }

        if(!HasCollidedAlongY){
            CurrentYOffset += yVelIncrement;
            this->CollisionRect->setY(CurrentYOffset);

            if(DetectCollision()){
                HasCollidedAlongY = true;

                CurrentYOffset -= yVelIncrement;
                this->CollisionRect->setY(CurrentYOffset);
                this->yVel = 0;

                //If we are jumping or falling, we need to reset our state to idle or walking
                if(this->CurrentState == JUMPING || this->CurrentState == FALLING){
                    if(abs(this->xVel) > 0){
                        this->SetState(WALKING);
                    }
                    else{
                        this->SetState(IDLING);
                    }
                }
            }
        }
    }

    this->setPos(this->pos() + this->CollisionRect->pos());

    emit(NewPositionAvailable(this->pos()));
}



bool Entity::DetectCollision()
{
    bool CollisionHasOccurred = false;
    //Loop through each colliding item
    QList<QGraphicsItem*> CollidingItems = this->scene()->collidingItems(this->CollisionRect);

    for(int i = 0; i < CollidingItems.length(); i++){
        bool IsEntity = dynamic_cast<Entity*>(CollidingItems[i]);
        bool IsWall = dynamic_cast<Wall*>(CollidingItems[i]);

        if(IsEntity){
            Entity *CollidingEntity = dynamic_cast<Entity*>(CollidingItems[i]);

            if(CollidingEntity != this){
                CollisionHasOccurred = true;
                break;
            }
        }

        if(IsWall){
            CollisionHasOccurred = true;
            break;
        }
    }

    return CollisionHasOccurred;
}

/*
 * Immediately moves this entity to the point specified
 */
void Entity::SetPosition(QPointF Pos)
{
    this->setPos(Pos);
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

    //Only update if the state has changed and this entity is not dead.
    if(this->CurrentState != State && this->CurrentState != EntityState::DEAD){
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
                //Handle any death events necessary
                this->HandleDeath();
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
            default:
                //Unknown state
                break;
        }
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

void Entity::SetScalingFactor(double ScalingFactor)
{
    this->Sprites->SetScalingFactor(ScalingFactor);
}

double Entity::GetScalingFactor()
{
    return this->Sprites->GetScalingFactor();
}

void Entity::SetAttackCollisionRect(QRectF AttackCollisionRect)
{
    this->AttackCollisionRect->setRect(AttackCollisionRect);
    this->AttackCollisionRectXOffset = AttackCollisionRect.x();
}

void Entity::SetAttackCollisionRect(double x, double y, double width, double height)
{
    this->AttackCollisionRect->setRect(x,y,width,height);
    this->AttackCollisionRectXOffset = x;
}

void Entity::ReverseDefaultDirection()
{
    this->Sprites->ReverseDefaultDirection();
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

void Entity::SetVerticalVelocity(double j)
{
    this->yVel = j;
}

void Entity::SetHorizontalVelocity(double i)
{
    this->xVel = i;
}

void Entity::SetDirection(Spritesheet::Direction Dir)
{
    this->Sprites->SetDirection(Dir);
}

void Entity::Walk(double Velocity)
{
    if(this->CurrentState != EntityState::DEAD){
        //If the velocity is negative, move left
        //Else move right
        if(Velocity < 0){
            //Negate the velocity, because the move left function will do it too
            //If this is not done, the entity will move right instead
            this->WalkLeft(-Velocity);
        }
        else{
            this->WalkRight(Velocity);
        }
    }
}

void Entity::StopWalking()
{
    //Set the xvel to 0
    this->xVel = 0;

    //If we are not jumping or falling, set the state to idling
    if(this->CurrentState != JUMPING && this->CurrentState != FALLING){
        this->SetState(IDLING);
    }
}

void Entity::WalkLeft(double Velocity)
{
    this->SetDirection(Spritesheet::LEFT);

    //Set our state to walking if we aren't jumping or falling
    if(this->CurrentState != JUMPING && this->CurrentState != FALLING){
        this->SetState(WALKING);
    }

    //The velocity must be set to negative, or else the entity will actually move right
    this->xVel = -Velocity;
}

void Entity::WalkRight(double Velocity)
{
    this->SetDirection(Spritesheet::RIGHT);

    //Set our state to walking if we aren't jumping or falling
    if(this->CurrentState != JUMPING && this->CurrentState != FALLING){
        this->SetState(WALKING);
    }

    this->xVel = Velocity;
}

double Entity::GetHealth()
{
    return this->Health;
}

double Entity::GetMaxHealth()
{
    return this->MaxHealth;
}

double Entity::GetAccelerationX()
{
    return this->xAccel;
}

double Entity::GetAccelerationY()
{
    return this->yAccel;
}

double Entity::GetVelocityX()
{
    return this->xVel;
}

double Entity::GetVelocityY()
{
    return this->yVel;
}

Spritesheet::Direction Entity::GetDirection()
{
    return this->Sprites->GetDirection();
}

QRectF Entity::GetRect()
{
    return this->boundingRect();
}

QPointF Entity::GetCenter()
{
    QPointF CenterPoint = this->pos() + this->boundingRect().center();
    return CenterPoint;
}

/*
 * Use padding to remove "whitespace" in your sprites
 * Some sprite frames may have extra space on the edges, which will result in
 * poor collision detection. Use this as a fudge factor to remove it in collision detection
 *
 * You will need X padding if the sprite collides with walls before actually touching them
 * You will need X padding if the sprite collides with walls before actually touching them
 */
void Entity::SetPadding(int PaddingX, int PaddingY)
{
    this->SetPaddingX(PaddingX);
    this->SetPaddingY(PaddingY);
}

/*
 * Use X padding to remove "whitespace" in your sprites (horizontally)
 * Some sprite frames may have extra space on the edges, which will result in
 * poor collision detection. Use this as a fudge factor to remove it in collision detection
 *
 * You will need X padding if the sprite collides with walls before actually touching them
 */
void Entity::SetPaddingX(int PaddingX)
{
    this->xPadding = PaddingX;
}

/*
 * Use Y padding to remove "whitespace" in your sprites (vertically)
 * Some sprite frames may have extra space on the edges, which will result in
 * poor collision detection. Use this as a fudge factor to remove it in collision detection
 *
 * You will need Y padding if the sprite "bounces up and down" slightly while standing still.
 */
void Entity::SetPaddingY(int PaddingY)
{
    this->yPadding = PaddingY;
}

void Entity::Attack()
{
    //Set this into an attacking state if we are not already in one
    if(this->CurrentState != ATTACKING){
        this->SetState(ATTACKING);

        //Clear our entity attack collision lists.
        this->ResetEntityLists();
    }
}

void Entity::Jump()
{
    //Check if we are not already jumping
    if(this->CurrentState != JUMPING && this->CurrentState != FALLING){
        //Begin a jump
        this->SetState(JUMPING);
        this->yVel = ENTITY_JUMP_STARTING_VELOCITY;
    }
}

/*
 * Generates floating damage text above the entity
 */
void Entity::GenerateDamageText(double Damage)
{
    //Generate damage text
    FloatingText *DamageText = new FloatingText(this->ScenePtr, this);

    DamageText->setPlainText(QString::number(Damage));

    //Move text to the center of this object
    double X = this->boundingRect().width()/2 - DamageText->boundingRect().width()/2;
    DamageText->setPos(X, 0);
}

/*
 * SLOT
 *
 * This slot is called externally when a dependent module has changed its state to idling (Spritesheet)
 */
void Entity::ChangeStateToIdle()
{
    if(this->CurrentState != EntityState::ATTACKING)
        this->SetState(IDLING);
    else
        this->SetState(WALKING);
}

/*
 * SLOT
 *
 * This slot is called externally when the dependent spritesheet has updated its frame.
 * This slot will update the internal frame with it
 */
void Entity::UpdateCurrentSprite(QPixmap NewSprite)
{
    this->setPixmap(NewSprite);
    if(this->CurrentState == ATTACKING){
        this->HandleAttackCollision();
    }
}

QPointF Entity::GetRelativeOffset(double Percentage)
{
    //Calculate the relative velocity
    double Vel_x = this->xVel * Percentage;
    double Vel_y = this->yVel * Percentage;

    //Calculate the position
    QPointF CollisionPoint = this->pos();
    CollisionPoint.setX(CollisionPoint.x() + Vel_x);
    CollisionPoint.setY(CollisionPoint.y() + Vel_y);

    return CollisionPoint;
}

void Entity::HandleAttackCollision()
{
    //Detect all enemies being attacked
    QList<Entity*> CollidingEntities = DetectAttackCollisions();

    //Filter enemies that have already been attacked in this attack
    QList<Entity*> FilteredEntities = FilterAlreadyDamagedEnemies(CollidingEntities);

    //Damage all enemies that have been detected and filtered
    this->DamageEntityList(FilteredEntities);
}

void Entity::RemoveEntityFromGame()
{
    emit(EntityHasBeenDeleted(this));
    this->scene()->removeItem(this);
    this->DeleteTimer = new QTimer(this);
    connect(this->DeleteTimer, SIGNAL(timeout()), this, SLOT(DeleteThisEntity()));
    this->DeleteTimer->start(1000);
}

void Entity::DeleteThisEntity()
{
    //delete(this);
}

void Entity::HandleDeath()
{
    qDebug() << "Deed";
    this->DecayTimer = new QTimer(this);
    connect(this->DecayTimer, SIGNAL(timeout()), this, SLOT(RemoveEntityFromGame()));
    this->DecayTimer->start(ENTITY_DECAY_TIME);

    emit(EntityHasDied(this));
    qDebug() << "Handle";
}

QList<Entity *> Entity::DetectAttackCollisions()
{
    //Update our attack collision rect's X value
    if(this->Sprites->GetDirection() == Spritesheet::LEFT){
        this->AttackCollisionRect->setX(-this->AttackCollisionRectXOffset);
    }
    else{
        this->AttackCollisionRect->setX(this->AttackCollisionRectXOffset);
    }

    //Detect any collisions with this object
    QList<QGraphicsItem*> CollidingItems = this->scene()->collidingItems(this->AttackCollisionRect);
    QList<Entity*> CollidingEntities;   //A list of all valid entities this entity collided with.

    for(int i = 0; i < CollidingItems.length(); i++){
        //Cast into entitys
        bool IsEntity = dynamic_cast<Entity*>(CollidingItems[i]);

        if(IsEntity){
            Entity* CollidingEntity = dynamic_cast<Entity*>(CollidingItems[i]);

            //One of the collisions will be against this entity, but we do not want that
            if(CollidingEntity != this){
                //A collision has occurred, append this entity to the list
                CollidingEntities.append(CollidingEntity);
            }
        }
    }

    return CollidingEntities;
}

QList<Entity *> Entity::FilterAlreadyDamagedEnemies(QList<Entity*> EntityList)
{
    //Construct a list of entities that have not been damaged yet
    QList<Entity*> FilteredEntities;


    //Loop through each entity in question
    for(int i = 0; i < EntityList.length(); i++){
        //Check if the entity has already been damaged
        if(!this->EntitiesAlreadyDamaged.contains(EntityList[i])){
            //If the entity has not been damaged, add it to the filtered entities list
            // and also add it to the damaged entities list
            FilteredEntities.append(EntityList[i]);
            EntitiesAlreadyDamaged.append(EntityList[i]);
        }
        else{
            //Do nothing if the entity has already been damaged.
        }
    }

    return FilteredEntities;
}

double Entity::DamageEntityList(QList<Entity *> EntityList)
{
    //Keep track of the total damage dealt
    double TotalDamageDealt = 0;
    //Loop through each entity and damage them
    for(int i = 0; i < EntityList.length(); i++){
        TotalDamageDealt += this->DamageTarget(EntityList[i], this->Damage);
    }

    return TotalDamageDealt;
}

void Entity::ResetEntityLists()
{
    this->EntitiesAlreadyDamaged.clear();
}

void Entity::Dump()
{
    qDebug() << this->pos();
}
