#include "terrain.h"

Terrain::Terrain(QGraphicsScene *ScenePtr)
{
    this->ScenePtr = ScenePtr;

    this->ScenePtr->addItem(this);

    //Set the Z value of this object to be in front of background
    this->setZValue(1);
}

void Terrain::SetSprite(QPixmap Pixmap)
{
    this->setPixmap(Pixmap);
}

void Terrain::SetSprite(QPixmap Pixmap, QRect Rect)
{
    QPixmap CopiedPixmap = Pixmap.copy(Rect);
    this->SetSprite(CopiedPixmap);
}

void Terrain::SetPosition(QPointF Pos)
{
    this->setPos(Pos);
}

void Terrain::SetScalingFactor(double ScalingFactor)
{
    this->ScalingFactor = ScalingFactor;
    int ScaledWidth = static_cast<int>(this->boundingRect().width() * ScalingFactor);
    int ScaledHeight = static_cast<int>(this->boundingRect().height() * ScalingFactor);
    this->setPixmap(this->pixmap().scaled(ScaledWidth,ScaledHeight));
}

double Terrain::GetScalingFactor()
{
    return this->ScalingFactor;
}
