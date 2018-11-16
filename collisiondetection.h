#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <QObject>

class CollisionDetection : public QObject
{
    Q_OBJECT
public:
    explicit CollisionDetection(QObject *parent = nullptr);

signals:

public slots:
};

#endif // COLLISIONDETECTION_H