#include "game.h"

Game::Game(Scene *GraphicsScenePtr, Camera *CameraPtr, QObject *parent) : QObject(parent)
{
    this->GraphicsScenePtr = GraphicsScenePtr;
    this->camera = CameraPtr;

    connect(this->GraphicsScenePtr, SIGNAL(KeyPressed(Qt::Key)), this, SLOT(KeyPressed(Qt::Key)));
    connect(this->GraphicsScenePtr, SIGNAL(KeyReleased(Qt::Key)), this, SLOT(KeyReleased(Qt::Key)));

    //Load the background
    this->LoadBackground();

    //Load the player
    this->LoadPlayer();

    //Load mobs
    this->LoadSkeletons();

    QTimer *MoveTimer = new QTimer();
    connect(MoveTimer, SIGNAL(timeout()), this, SLOT(UpdateEntities()));
    MoveTimer->start(50);

    //Load terrain
    this->LoadTerrain();

}

//Returns the internal list of entity pointers
QList<Entity *> Game::GetEntities()
{
    return this->EntityList;
}

void Game::UpdateEntities()
{
    qDebug() << "Update";
    for(int i = 0; i < EntityList.length(); i++){
        //Figure out what type of object the entity is
        bool IsNPC = dynamic_cast<NPC*>(EntityList[i]);

        if(IsNPC){
            //The object is an NPC, use the NPC's update function
            NPC *npc = dynamic_cast<NPC*>(EntityList[i]);
            npc->UpdateAI();
        }
        else{
            //The object is an ordinary entity
            EntityList[i]->Move();
        }

    }
    qDebug() << "list";
}

void Game::RemoveEntity(Entity *RemovedEntity)
{
    qDebug() << "Remove";
    qDebug() << RemovedEntity->pos();
    this->EntityList.removeAll(RemovedEntity);
    qDebug() << "Done";
}

void Game::KeyPressed(Qt::Key Key)
{
    this->ActionKeyPressed(Key);
}

void Game::KeyReleased(Qt::Key Key)
{
    this->ActionKeyReleased(Key);
}

void Game::SetCamera(Camera *CameraPtr)
{
    this->camera = CameraPtr;
}

void Game::ActionKeyPressed(Qt::Key Key)
{
    qDebug() << "Pressed";
    switch(Key){
        case Qt::Key_Up:
            this->Player->Jump();
            break;
        case Qt::Key_Down:
            //This cannot do anything. this->entity->SetVerticalVelocity(6);
            break;
        case Qt::Key_Left:
            this->Player->Walk(-ENTITY_WALK_DEFAULT_SPEED);
            break;
        case Qt::Key_Right:
            this->Player->Walk();
            break;
        case Qt::Key_A:
            this->Player->Attack();
            break;
        default:
            //Key is invalid, do nothing
            break;
    }
    qDebug() << "pressfinished";
}

void Game::ActionKeyReleased(Qt::Key Key)
{
    qDebug() << "released";
    switch(Key){
        case Qt::Key_Up:
            //Do not stop the velocity. this->entity->SetVerticalVelocity(0);
            break;
        case Qt::Key_Down:
            //This cannot do anything. this->entity->SetVerticalVelocity(0);
            break;
        case Qt::Key_Left:
            if(this->Player->GetVelocityX() < 0){
                this->Player->StopWalking();
            }
            break;
        case Qt::Key_Right:
            if(this->Player->GetVelocityX() > 0){
                this->Player->StopWalking();
            }
            break;
        default:
            //Key is invalid, do nothing
            break;
    }
    qDebug() << "Relefinished";
}

void Game::LoadBackground()
{
}

void Game::LoadSkeletons()
{
    QList<QPointF> SkeletonPositions;
    SkeletonPositions.append(QPointF(500,-30));
    //SkeletonPositions.append(QPointF(500,200));
    //SkeletonPositions.append(QPointF(600,-30));
    //SkeletonPositions.append(QPointF(700,200));
    //SkeletonPositions.append(QPointF(800,-30));
    //SkeletonPositions.append(QPointF(900,200));

    for(int i = 0; i < SkeletonPositions.length(); i++){
        NPC *NewNPC = new NPC(this->GraphicsScenePtr, this, nullptr);

        //Load the pixmap
        Spritesheet *s = NewNPC->GetSpritesheet();

        s->SetSpriteSource(QPixmap(":/sprites/assets/Skeleton.png"));

        //Idle
        QList<QRect> SpriteRect;
        SpriteRect.append(QRect(54,0,300,450));
        SpriteRect.append(QRect(377,0,300,450));
        SpriteRect.append(QRect(697,0,300,450));
        SpriteRect.append(QRect(1032,0,300,450));
        SpriteRect.append(QRect(1384,0,300,450));
        SpriteRect.append(QRect(54,464,300,450));
        SpriteRect.append(QRect(377,464,300,450));
        SpriteRect.append(QRect(697,464,300,450));
        SpriteRect.append(QRect(1032,464,300,450));
        SpriteRect.append(QRect(1384,464,300,450));

        SpriteRect.append(QRect(1384,464,300,450));
        SpriteRect.append(QRect(1032,464,300,450));
        SpriteRect.append(QRect(697,464,300,450));
        SpriteRect.append(QRect(377,464,300,450));
        SpriteRect.append(QRect(54,464,300,450));
        SpriteRect.append(QRect(1384,0,300,450));
        SpriteRect.append(QRect(1032,0,300,450));
        SpriteRect.append(QRect(697,0,300,450));
        SpriteRect.append(QRect(377,0,300,450));
        SpriteRect.append(QRect(54,0,300,450));

        //Set the idle sprite, and default sprites that we wont use to idle.
        s->SetSprite(Spritesheet::IDLE, SpriteRect);
        s->SetSprite(Spritesheet::CROUCH, SpriteRect);
        s->SetSprite(Spritesheet::FALL, SpriteRect);
        s->SetSprite(Spritesheet::JUMP, SpriteRect);
        s->SetSprite(Spritesheet::SHOOT, SpriteRect);

        //Attack sprite
        SpriteRect.clear();
        SpriteRect.append(QRect(0,1000,300,500));
        SpriteRect.append(QRect(345,1000,300,500));
        SpriteRect.append(QRect(707,1000,300,500));
        SpriteRect.append(QRect(1074,1000,288,500));
        SpriteRect.append(QRect(1360,1000,325,500));
        SpriteRect.append(QRect(0,1500,300,500));
        SpriteRect.append(QRect(345,1500,300,500));
        SpriteRect.append(QRect(707,1500,300,500));
        SpriteRect.append(QRect(1074,1500,300,500));
        SpriteRect.append(QRect(1350,1500,325,500));
        s->SetSprite(Spritesheet::ATTACK, SpriteRect);
        NewNPC->SetAttackCollisionRect(QRectF(0,0,100,500));


        //Death sprite
        SpriteRect.clear();
        SpriteRect.append(QRect(850,3526,325,500));
        s->SetSprite(Spritesheet::DEATH, SpriteRect);


        //Walk sprite
        SpriteRect.clear();
        SpriteRect.append(QRect(54,2021,260,450));
        SpriteRect.append(QRect(329,2021,260,450));
        SpriteRect.append(QRect(605,2021,260,450));
        SpriteRect.append(QRect(886,2021,260,450));
        SpriteRect.append(QRect(1168,2021,260,450));
        SpriteRect.append(QRect(54,2479,260,450));
        SpriteRect.append(QRect(329,2479,260,450));
        SpriteRect.append(QRect(605,2479,260,450));
        SpriteRect.append(QRect(886,2479,260,450));
        SpriteRect.append(QRect(1168,2479,260,450));
        s->SetSprite(Spritesheet::WALK, SpriteRect);

        NewNPC->SetScalingFactor(0.20);
        NewNPC->ReverseDefaultDirection();
        NewNPC->SetDirection(Spritesheet::LEFT);

        NewNPC->UpdateSpritesheet(s);
        NewNPC->SetAIState(NPC::IDLING);
        NewNPC->setPos(SkeletonPositions[i]);
        NewNPC->SetAttackRange(90);
        NewNPC->UpdateAI();

        connect(NewNPC, SIGNAL(EntityHasBeenDeleted(Entity*)), this, SLOT(RemoveEntity(Entity*)));

        this->EntityList.append(NewNPC);
    }
}

void Game::LoadPlayer()
{
    this->Player = new Entity(GraphicsScenePtr);
    this->Player->setPos(100,100);
    Spritesheet *sheet = this->Player->GetSpritesheet();
    this->Player->SetPadding(20,6);
    camera->FollowEntity(this->Player);
    /*
     * Spritesheet test
     */

    //Test creating a new sheet and giving it a source
    QPixmap Sprite(":/sprites/assets/elisa.png");
    sheet->SetSpriteSource(Sprite);

    //Test adding an idle sprite
    QList<QRect> RectList;
    RectList.append(QRect(4,4,53,53));
    RectList.append(QRect(61,4,53,53));
    RectList.append(QRect(118,4,53,53));
    sheet->SetSprite(Spritesheet::IDLE, RectList);

    //Test adding an attack sprite
    RectList.clear();
    RectList.append(QRect(17,187,53,53));
    RectList.append(QRect(110,187,53,53));
    RectList.append(QRect(202,187,53,53));
    RectList.append(QRect(35,252,53,53));
    RectList.append(QRect(110,252,53,53));
    RectList.append(QRect(202,252,53,53));
    sheet->SetSprite(Spritesheet::ATTACK, RectList);
    sheet->SetCurrentSpritesheet(Spritesheet::ATTACK);
    this->Player->SetAttackCollisionRect(20,20,60,90);

    /*
     * Test adding a walk sprite
     */
    RectList.clear();
    RectList.append(QRect(5,121,53,53));
    RectList.append(QRect(62,121,53,53));
    RectList.append(QRect(119,121,53,53));
    RectList.append(QRect(176,121,53,53));
    RectList.append(QRect(233,121,53,53));
    RectList.append(QRect(290,121,53,53));
    RectList.append(QRect(347,121,53,53));
    RectList.append(QRect(404,121,53,53));
    sheet->SetSprite(Spritesheet::WALK, RectList);

    /*
     * Death sprite
     */
    RectList.clear();
    RectList.append(QRect(238,4,53,53));
    sheet->SetSprite(Spritesheet::DEATH, RectList);

    /*
     * Jump
     */
    RectList.clear();
    RectList.append(QRect(66,62,53,53));
    sheet->SetSprite(Spritesheet::JUMP, RectList);

    this->Player->ChangeStateToIdle();
    this->Player->SetScalingFactor(2);
    this->EntityList.append(this->Player);
    connect(this->Player, SIGNAL(EntityHasBeenDeleted(Entity*)), this, SLOT(RemoveEntity(Entity*)));
}

void Game::LoadTerrain()
{
    Wall *t;
    Doodad *d;
    //Starting left wall
    for(int i = 0; i < 13; i++){
        t = new Wall(GraphicsScenePtr);
        t->SetSprite(QPixmap(":/walls/assets/twilight-tiles.png"), QRect(161,97,13,45));
        t->SetPosition(QPointF(0,(90*i)));
        t->SetScalingFactor(2);
    }

    //Starting platform
    for(int i = 0; i < 13; i++){
        t = new Wall(GraphicsScenePtr);
        t->SetSprite(QPixmap(":/walls/assets/twilight-tiles.png"), QRect(113,145,45,13));
        t->SetPosition(QPointF((90 * i),300));
        t->SetScalingFactor(2);
    }

    //Starting platform arches
    for(int i = 0; i < 3; i++){
        d = new Doodad(GraphicsScenePtr);
        d->SetSprite(QPixmap(":/walls/assets/Tiles-Background-Removed.png"), QRect(24,64,87 - 24,175 - 64));
        d->SetPosition(QPointF((i * 400) + 26,326));
        d->SetScalingFactor(2);
    }

    //Ground
    for(int i = 0; i < 20; i++){
        t = new Wall(GraphicsScenePtr);
        t->SetSprite(QPixmap(":/walls/assets/twilight-tiles.png"), QRect(112,0,159-112,47));
        t->SetPosition(QPointF((94 * i) + 26, 548));
        t->SetScalingFactor(2);
    }
    for(int i = 0; i < 20; i++){
        t = new Wall(GraphicsScenePtr);
        t->SetSprite(QPixmap(":/walls/assets/twilight-tiles.png"), QRect(112,0,159-112,47));
        t->SetPosition(QPointF((94 * i) + 26, 642));
        t->SetScalingFactor(2);
    }
}
