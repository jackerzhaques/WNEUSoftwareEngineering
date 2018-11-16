#include "terrain.h"

Terrain::Terrain(QGraphicsScene *ScenePtr)
{
    this->ScenePtr = ScenePtr;

    this->PixmapItem = new QGraphicsPixmapItem();

    this->ScenePtr->addItem(this->PixmapItem);
}

void Terrain::SetSprite(QPixmap Pixmap)
{
    this->PixmapItem->setPixmap(Pixmap);
    this->UnscaledSprite = Pixmap.copy();
}

void Terrain::SetSprite(QPixmap Pixmap, QRect Rect)
{
    QPixmap CopiedPixmap = Pixmap.copy(Rect);
    this->SetSprite(CopiedPixmap);
}

void Terrain::ScaleSprite(double ScalingFactor)
{
    int NewWidth = (int)(this->UnscaledSprite.width() * ScalingFactor);
    int NewHeight = (int)(this->UnscaledSprite.height() * ScalingFactor);
    QPixmap ScaledSprite = this->UnscaledSprite.scaled(NewWidth, NewHeight);
    this->PixmapItem->setPixmap(ScaledSprite);
}

void Terrain::SetPosition(QPointF Pos)
{
    this->PixmapItem->setPos(Pos);
}
