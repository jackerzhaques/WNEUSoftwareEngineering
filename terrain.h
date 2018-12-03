#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QDebug>

class Terrain : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Terrain(QGraphicsScene *ScenePtr);
    void SetSprite(QPixmap Pixmap);
    void SetSprite(QPixmap Pixmap, QRect Rect);
    void SetPosition(QPointF Pos);
    void SetScalingFactor(double ScalingFactor);
    double GetScalingFactor();

signals:

public slots:

private:
    QGraphicsScene *ScenePtr = nullptr;
    double ScalingFactor = 1;
};

#endif // TERRAIN_H
