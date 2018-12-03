#include "collisiondetection.h"

CollisionDetection::CollisionDetection(QObject *parent) : QObject(parent)
{

}

/*
 * Returns the intersection between two rectangles
 * NOTE: The rectangles must be mapped to the same scene
 * NOTE: Use this function statically
 */
QRectF CollisionDetection::RectIntersection(QRectF Rect1, QRectF Rect2)
{
    QRectF IntersectingRect(0,0,0,0);

    if(Rect1.intersects(Rect2)){
        /*
        //Calculate the maximum center distance between the two rectangles
        double MaxCenterDistanceX = (Rect1.width() + Rect2.width())/2;
        double MaxCenterDistanceY = (Rect1.height() + Rect2.height())/2;

        //Calculate the distance between the rect centers
        QPointF CenterDistances = Rect2.center() - Rect1.center();  //The point's X and Y are the distances

        //Calculate the percentage of center distance the intersection covers
        double PercentageX = CenterDistances.x() - MaxCenterDistanceX;
        double PercentageY = CenterDistances.y() - MaxCenterDistanceY;

        //Calculate the width and height of the intersecting rectangle as inversely proportional to the
        //percentage of center distance
        double Width = (1 - PercentageX) * MaxCenterDistanceX;
        double Height = (1 - PercentageY) * MaxCenterDistanceY;
        */

        double X = Max(Rect1.x(),Rect2.x());
        double Y = Max(Rect1.y(),Rect2.y());

        double RightSide = Min(Rect1.x() + Rect1.width(),Rect2.x() + Rect2.width());
        double BottomSide = Min(Rect1.y() + Rect1.height(), Rect2.y() + Rect2.height());

        double Width = RightSide - X;
        double Height = BottomSide - Y;
        IntersectingRect.setRect(X,Y,Width,Height);
        /*
        IntersectingRect.setBottom(Min(Rect1.bottom(),Rect2.bottom()));
        IntersectingRect.setTop(Max(Rect1.top(), Rect2.top()));
        IntersectingRect.setLeft(Max(Rect1.left(),Rect2.left()));
        IntersectingRect.setRight(Min(Rect1.right(), Rect2.right()));
        */
    }

    return IntersectingRect;
}

double CollisionDetection::Min(double Val1, double Val2)
{
    if(Val1 < Val2) return Val1;
    else return Val2;
}

double CollisionDetection::Max(double Val1, double Val2)
{
    if(Val1 > Val2) return Val1;
    else return Val2;
}

CollisionDetection::CollidingSide CollisionDetection::GetCollidingSide(QRectF CollidingRectangle, QRectF ObjectRectangle)
{
    CollidingSide Side = UNKNOWN;
    //The intersection will be a TOP or BOTTOM if the rectangle has a greater
    //width than height
    if(CollidingRectangle.width() > CollidingRectangle.height()){
        Side = GetCollidingVerticalSide(CollidingRectangle, ObjectRectangle);
    }
    else{
        Side = GetCollidingHorizontalSide(CollidingRectangle, ObjectRectangle);
    }

    return Side;
}

CollisionDetection::CollidingSide CollisionDetection::GetCollidingVerticalSide(QRectF CollidingRectangle, QRectF ObjectRectangle)
{
    CollidingSide Side = UNKNOWN;

    //If the collision rectangle is above the object's rectangle, it is a top collision
    //Otherwise it is a bottom collision
    if(ObjectRectangle.center().y() > CollidingRectangle.center().y()){
        Side = TOP;
    }
    else{
        Side = BOTTOM;
    }

    return Side;
}

CollisionDetection::CollidingSide CollisionDetection::GetCollidingHorizontalSide(QRectF CollidingRectangle, QRectF ObjectRectangle)
{
    CollidingSide Side = UNKNOWN;

    //If the collision rectangle is to the left the object's rectangle, it is a left collision
    //Otherwise it is a right collision
    if(ObjectRectangle.center().x() > CollidingRectangle.center().x()){
        Side = LEFT;
    }
    else{
        Side = RIGHT;
    }

    return Side;
}
