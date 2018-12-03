#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

#define MOVE_UP_VELOCITY                -0.5       //Velocity of text as it moves up
#define DEFAULT_TEXT_TIME               2000    //Total number of milliseconds the text lasts
#define DEFAULT_TEXT_UPDATE_INTERVAL    10     //Number of milliseconds between each text update

//The amount of alpha to remove each update
#define DEFAULT_TEXT_GRADIENT           1.0 / (DEFAULT_TEXT_TIME / DEFAULT_TEXT_UPDATE_INTERVAL)

class FloatingText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit FloatingText(QGraphicsScene *ScenePtr, QGraphicsItem *parent = nullptr);

signals:

public slots:
    void MoveUp();
    void Remove();

private:
    QGraphicsScene *ScenePtr = nullptr;
    QTimer *MoveTimer = nullptr;        //Timer moves text up periodically
    QTimer *DeleteTimer = nullptr;      //Timer deletes text after a certain amount of time
};

#endif // FLOATINGTEXT_H
