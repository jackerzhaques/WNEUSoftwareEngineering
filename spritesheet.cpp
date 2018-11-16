#include "spritesheet.h"

Spritesheet::Spritesheet(QObject *parent) : QObject(parent)
{
    this->SpriteTimer = new QTimer(this);

    connect(this->SpriteTimer, SIGNAL(timeout()), this, SLOT(UpdateSpriteAnimation()));
    this->SpriteTimer->start(this->SpriteTimePeriod);
}

void Spritesheet::SetDirection(Spritesheet::Direction Dir)
{
    this->Dir = Dir;
}

void Spritesheet::SetSpriteTimePeriod(int TimePeriodInMs)
{
    this->SpriteTimePeriod = TimePeriodInMs;
    this->SpriteTimer->setInterval(TimePeriodInMs);
}

void Spritesheet::StartSpriteTimer()
{
    this->SpriteTimer->start(this->SpriteTimePeriod);
}

void Spritesheet::PauseSpriteTimer()
{
    this->SpriteTimer->stop();
}

void Spritesheet::SetSpriteSource(QPixmap SpriteSource)
{
    this->SpriteSource = SpriteSource;
}

void Spritesheet::SetSprite(Spritesheet::SpriteType Type, QList<QRect> RectList)
{
    //Set the new sprite list for the appropriate sprite
    switch(Type){
        case IDLE:
            this->IdleSpriteList = RectList;
            break;
        case WALK:
            this->WalkSpriteList = RectList;
            break;
        case ATTACK:
            this->AttackSpriteList = RectList;
            break;
        case SHOOT:
            this->ShootSpriteList = RectList;
            break;
        case DEATH:
            this->DeathSpriteList = RectList;
            break;
        case CROUCH:
            this->CrouchSpriteList = RectList;
            break;
        case JUMP:
            this->JumpSpriteList = RectList;
            break;
        case FALL:
            this->FallSpriteList = RectList;
            break;
        case CHANNEL:
            this->ChannelSpriteList = RectList;
            break;
    }

    //Update our current sprite sheet
    this->SetCurrentSpritesheet(Type);
}

void Spritesheet::SetSpriteLoops(Spritesheet::SpriteType Type, bool SpriteLoops)
{
    //Set the new sprite list loops for the appropriate sprite
    switch(Type){
        case IDLE:
            this->IdleSpriteLoops = SpriteLoops;
            break;
        case WALK:
            this->WalkSpriteLoops = SpriteLoops;
            break;
        case ATTACK:
            this->AttackSpriteLoops = SpriteLoops;
            break;
        case SHOOT:
            this->ShootSpriteLoops = SpriteLoops;
            break;
        case DEATH:
            this->DeathSpriteLoops = SpriteLoops;
            break;
        case CROUCH:
            this->CrouchSpriteLoops = SpriteLoops;
            break;
        case JUMP:
            this->JumpSpriteLoops = SpriteLoops;
            break;
        case FALL:
            this->FallSpriteLoops = SpriteLoops;
            break;
        case CHANNEL:
            this->ChannelSpriteLoops = SpriteLoops;
            break;
    }
}

void Spritesheet::SetCurrentSpritesheet(Spritesheet::SpriteType Type)
{
    //Reset the index
    this->CurrentSpriteIndex = 0;

    //Set the current sprite list
    switch(Type){
        case IDLE:
            this->CurrentSpriteList = this->IdleSpriteList;
            this->CurrentSpriteLoops = this->IdleSpriteLoops;
            break;
        case WALK:
            this->CurrentSpriteList = this->WalkSpriteList;
            this->CurrentSpriteLoops = this->WalkSpriteLoops;
            break;
        case ATTACK:
            this->CurrentSpriteList = this->AttackSpriteList;
            this->CurrentSpriteLoops = this->AttackSpriteLoops;
            break;
        case SHOOT:
            this->CurrentSpriteList = this->ShootSpriteList;
            this->CurrentSpriteLoops = this->ShootSpriteLoops;
            break;
        case DEATH:
            this->CurrentSpriteList = this->DeathSpriteList;
            this->CurrentSpriteLoops = this->DeathSpriteLoops;
            break;
        case CROUCH:
            this->CurrentSpriteList = this->CrouchSpriteList;
            this->CurrentSpriteLoops = this->CrouchSpriteLoops;
            break;
        case JUMP:
            this->CurrentSpriteList = this->JumpSpriteList;
            this->CurrentSpriteLoops = this->JumpSpriteLoops;
            break;
        case FALL:
            this->CurrentSpriteList = this->FallSpriteList;
            this->CurrentSpriteLoops = this->FallSpriteLoops;
            break;
        case CHANNEL:
            this->CurrentSpriteList = this->ChannelSpriteList;
            this->CurrentSpriteLoops = this->ChannelSpriteLoops;
            break;
    }

    //Update the current sprite view
    this->GetCurrentSprite();
}

void Spritesheet::UpdateSpriteAnimation()
{
    //Go to the next sprite
    this->AdvanceSpriteIndex();

    //Locate the pixmap of our next sprite.
    this->GetCurrentSprite();

    //Check any collision if this is an attack frame
    if(this->InAttackState){
        if(this->CurrentSpriteIndex >= this->MinIndexForAttackCollision && this->CurrentSpriteIndex <= this->MaxIndexForAttackCollision){
            this->CheckAttackCollision();
        }
    }
}

int Spritesheet::AdvanceSpriteIndex()
{
    QList<QRect> List = this->CurrentSpriteList;

    this->CurrentSpriteIndex++;

    //Wrap around if necessary
    if(this->CurrentSpriteIndex >= List.length()){
        if(this->CurrentSpriteLoops){
            this->CurrentSpriteIndex = 0;
        }
        else{
            //The sprite does not loop, force it into an idle state
            this->SetCurrentSpritesheet(SpriteType::IDLE);
            emit(StateChangedToIdle());
        }
    }

    return this->CurrentSpriteIndex;
}

/*
 * TODO: Implement this function once collision detection is implemented
 */
double Spritesheet::CheckAttackCollision()
{
    return 0;
}

void Spritesheet::MirrorSprite()
{
    //Mirror the sprite across the Y axis if we are facing left
    //Sprites are assumed to be facing right normally.
    if(this->Dir == Direction::LEFT){
        this->CurrentSprite = this->CurrentSprite.transformed(QTransform().scale(-1,1));
    }
}

QPixmap Spritesheet::GetCurrentSprite()
{
    QList<QRect> SpriteList = this->CurrentSpriteList;
    int CurrentIndex = this->CurrentSpriteIndex;

    QRect CurrentSpriteRect = SpriteList.at(CurrentIndex);
    this->CurrentSprite = this->SpriteSource.copy(CurrentSpriteRect);

    //Mirror the sprite if we are facing a different direction
    this->MirrorSprite();

    emit(SpriteChanged(this->CurrentSprite));

    return CurrentSprite;
}
