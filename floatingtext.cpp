#include "floatingtext.h"

FloatingText::FloatingText(QGraphicsScene *ScenePtr, QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    this->ScenePtr = ScenePtr;

    this->MoveTimer = new QTimer(this);
    connect(this->MoveTimer, SIGNAL(timeout()), this, SLOT(MoveUp()));
    this->MoveTimer->start(DEFAULT_TEXT_UPDATE_INTERVAL);

    this->DeleteTimer = new QTimer(this);
    connect(this->DeleteTimer, SIGNAL(timeout()), this, SLOT(Remove()));
    this->DeleteTimer->start(DEFAULT_TEXT_TIME);

    this->setDefaultTextColor(QColor::fromRgb(255,255,255));
}

void FloatingText::MoveUp()
{
    qDebug() << "Up!";
    this->setY(this->y() + MOVE_UP_VELOCITY);
    this->setOpacity(this->opacity() - DEFAULT_TEXT_GRADIENT);
    qDebug() << "Move";
}

void FloatingText::Remove()
{
    qDebug() << "text";
    delete(this);
    qDebug() << "delete";
}
