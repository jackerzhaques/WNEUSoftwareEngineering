#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

/*
 * This file provides global collision detection functions.
 */

#include <QObject>
#include <QRectF>
#include <QDebug>

class CollisionDetection : public QObject
{
    Q_OBJECT
public:
    explicit CollisionDetection(QObject *parent = nullptr);
    static QRectF RectIntersection(QRectF Rect1, QRectF Rect2);
    static double Min(double Val1, double Val2);
    static double Max(double Val1, double Val2);

    enum CollidingSide{
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        UNKNOWN
    };

    static CollidingSide GetCollidingSide(QRectF CollidingRectangle, QRectF ObjectRectangle);

signals:

public slots:
private:
    static CollidingSide GetCollidingVerticalSide(QRectF CollidingRectangle, QRectF ObjectRectangle);
    static CollidingSide GetCollidingHorizontalSide(QRectF CollidingRectangle, QRectF ObjectRectangle);
};

#endif // COLLISIONDETECTION_H
