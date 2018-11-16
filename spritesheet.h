#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <QObject>
#include <QList>
#include <QPixmap>
#include <QRect>
#include <QTimer>
#include <QTransform>

#include <QDebug>

class Spritesheet : public QObject
{
    Q_OBJECT
public:
    explicit Spritesheet(QObject *parent = nullptr);

    enum Direction{
        LEFT,
        RIGHT
    };

    enum SpriteType{
        IDLE,
        WALK,
        ATTACK,
        SHOOT,
        DEATH,
        CROUCH,
        JUMP,
        FALL,
        CHANNEL
    };

    void SetDirection(Direction Dir);
    void SetSpriteTimePeriod(int TimePeriodInMs);
    void StartSpriteTimer();
    void PauseSpriteTimer();

    /*
     * Functions for setting and updating the spritesheets
     */
    void SetSpriteSource(QPixmap SpriteSource);
    void SetSprite(SpriteType Type, QList<QRect> RectList);
    void SetSpriteLoops(SpriteType Type, bool SpriteLoops);
    void SetCurrentSpritesheet(SpriteType Type);

    //The current pixmap of the spritesheet
    QPixmap CurrentSprite;

signals:
    void StateChangedToIdle();
    void SpriteChanged(QPixmap Sprite);

public slots:
    void UpdateSpriteAnimation();

private:
    int AdvanceSpriteIndex();
    double CheckAttackCollision();
    void MirrorSprite();
    QPixmap GetCurrentSprite();

    Direction Dir = RIGHT;
    QList<QRect> CurrentSpriteList;
    int CurrentSpriteIndex = 0;
    bool CurrentSpriteLoops = false;

    QRect AttackCollisionRect;
    int SpriteTimePeriod = 100; //ms
    QTimer *SpriteTimer = nullptr;

    QPixmap SpriteSource;
    QList<QRect> IdleSpriteList;
    QList<QRect> WalkSpriteList;
    QList<QRect> AttackSpriteList;
    QList<QRect> ShootSpriteList;
    QList<QRect> DeathSpriteList;
    QList<QRect> CrouchSpriteList;
    QList<QRect> JumpSpriteList;
    QList<QRect> FallSpriteList;
    QList<QRect> ChannelSpriteList;

    bool IdleSpriteLoops = true;
    bool WalkSpriteLoops = true;
    bool AttackSpriteLoops = false;
    bool ShootSpriteLoops = false;
    bool DeathSpriteLoops = false;
    bool CrouchSpriteLoops = false;
    bool JumpSpriteLoops = true;
    bool FallSpriteLoops = true;
    bool ChannelSpriteLoops = true;

    bool InAttackState = false;
    int MinIndexForAttackCollision = 3;
    int MaxIndexForAttackCollision = 6;

};

#endif // SPRITESHEET_H
