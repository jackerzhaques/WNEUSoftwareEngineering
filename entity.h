#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QGraphicsItem>
#include <QWidget>
#include <QPointF>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsRectItem>

#include "spritesheet.h"
#include "terrain.h"
#include "collisiondetection.h"
#include "floatingtext.h"
#include "wall.h"

#define ENTITY_MOVE_STEP_PERCENTAGE         0.5
#define ENTITY_WALK_DEFAULT_SPEED           30
#define ENTITY_JUMP_START_THRESHOLD         0.1
#define ENTITY_JUMP_STARTING_VELOCITY       -60
#define ENTITY_DECAY_TIME                   3000    //Decay time of entity in milliseconds

class Entity : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Entity(QGraphicsScene *ScenePtr, QGraphicsItem *parent = nullptr);

    //Copy's the object's position, scene pointer, and spritesheet over to this object.
    explicit Entity(Entity* CopyEntityPtr);

    void Move();
    bool DetectCollision();
    void SetPosition(QPointF Pos);
    double TakeDamage(double Damage);
    double DamageTarget(Entity *Target, double Damage);
    double Heal(double HealAmount);
    bool CheckDeath();

    enum EntityState{
        IDLING,
        WALKING,
        ATTACKING,
        SHOOTING,
        DEAD,
        CROUCHING,
        JUMPING,
        FALLING,
        CHANNELING,
        UNKNOWN
    };

    void SetState(EntityState State);
    Spritesheet* GetSpritesheet();
    void UpdateSpritesheet(Spritesheet *Sprite);
    void SetScalingFactor(double ScalingFactor);
    double GetScalingFactor();
    void SetAttackCollisionRect(QRectF AttackCollisionRect);
    void SetAttackCollisionRect(double x, double y, double width, double height);
    void ReverseDefaultDirection();

    void SetAcceleration(double i, double j);
    void SetVelocity(double i, double j);
    void SetVerticalVelocity(double j);
    void SetHorizontalVelocity(double i);
    void SetDirection(Spritesheet::Direction Dir);
    double GetAccelerationX();
    double GetAccelerationY();
    double GetVelocityX();
    double GetVelocityY();
    Spritesheet::Direction GetDirection();

    QRectF GetRect();
    QPointF GetCenter();

    void SetPadding(int PaddingX, int PaddingY);
    void SetPaddingX(int PaddingX);
    void SetPaddingY(int PaddingY);

    //Mechanic functions
    void Attack();
    void Jump();
    void Walk(double Velocity = ENTITY_WALK_DEFAULT_SPEED);
    void StopWalking();
    void WalkLeft(double Velocity = ENTITY_WALK_DEFAULT_SPEED);
    void WalkRight(double Velocity = ENTITY_WALK_DEFAULT_SPEED);

    //Getting vitals
    double GetHealth();
    double GetMaxHealth();

signals:
    void NewPositionAvailable(QPointF Position);
    void EntityHasBeenDeleted(Entity* RemovedEntity);
    void EntityHasDied(Entity* DeadEntity);

public slots:
    void GenerateDamageText(double Damage);
    void ChangeStateToIdle();
    void UpdateCurrentSprite(QPixmap NewSprite);
    void HandleAttackCollision();
    void RemoveEntityFromGame();
    void DeleteThisEntity();


private:
    double Health = 5;
    double MaxHealth = 5;
    double Mana = 0;
    double MaxMana = 0;
    double Defense = 1;
    double Damage = 2;

    //Death control
    QTimer *DecayTimer = nullptr;   //After this timer expires, the entity is removed from the game.
    QTimer *DeleteTimer = nullptr;  //After this timer expires, the entity will be deleted
    void HandleDeath();

    //Position control
    double xAccel = 0;
    double yAccel = 5.0;  //Gravity constant
    double xVel = 0;
    double yVel = 0;
    QPointF GetRelativeOffset(double Percentage);

    //Animation
    Spritesheet *Sprites = nullptr;
    EntityState CurrentState = UNKNOWN;

    //Pointers to external sources
    QGraphicsScene *ScenePtr;

    //Collision Detection
    QGraphicsRectItem *CollisionRect = nullptr;
    QGraphicsRectItem *AttackCollisionRect = nullptr;
    double xPadding = 0;
    double yPadding = 0;
    double AttackCollisionRectXOffset = 0;

    //Attack collision detection
    QList<Entity*> EntitiesAlreadyDamaged;
    QList<Entity*> DetectAttackCollisions();
    QList<Entity*> FilterAlreadyDamagedEnemies(QList<Entity*> EntityList);
    double DamageEntityList(QList<Entity*> EntityList);
    void ResetEntityLists();

    //Debug functions
    void Dump();
};

#endif // ENTITY_H
