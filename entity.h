#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include "spritesheet.h"

class Entity : public QObject
{
    Q_OBJECT
public:
    explicit Entity(QGraphicsScene *ScenePtr, QObject *parent = nullptr);

    void Move();
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
        CHANNELING
    };

    void SetState(EntityState State);
    Spritesheet* GetSpritesheet();
    void UpdateSpritesheet(Spritesheet *Sprite);

    void SetAcceleration(double i, double j);
    void SetVelocity(double i, double j);

signals:

public slots:
    void GenerateDamageText(double Damage);
    void ChangeStateToIdle();
    void UpdateCurrentSprite(QPixmap NewSprite);

private:
    double Health = 0;
    double MaxHealth = 0;
    double Mana = 0;
    double MaxMana = 0;
    double Defense = 0;

    //Position control
    double xAccel = 0;
    double yAccel = 0;
    double xVel = 0;
    double yVel = 0;

    //Animation
    Spritesheet *Sprites = nullptr;
    EntityState CurrentState = IDLING;
    QGraphicsPixmapItem *Graphic = nullptr;

    //Pointers to external sources
    QGraphicsScene *ScenePtr;
};

#endif // ENTITY_H
