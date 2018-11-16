#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRect>
#include <QPoint>

class Terrain : public QObject
{
    Q_OBJECT
public:
    explicit Terrain(QGraphicsScene *ScenePtr);
    void SetSprite(QPixmap Pixmap);
    void SetSprite(QPixmap Pixmap, QRect Rect);
    void ScaleSprite(double ScalingFactor);
    void SetPosition(QPointF Pos);

signals:

public slots:

private:
    QGraphicsPixmapItem *PixmapItem = nullptr;
    QGraphicsScene *ScenePtr = nullptr;
    QPixmap UnscaledSprite;
};

#endif // TERRAIN_H
