#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

/*
 * This class inherits QGraphicsScene and for all purposes functions as a normal
 * QGraphicsScene. The purpose of this class is to override the keyPressEvent and
 * keyReleaseEvent to emit signals when they are pressed, so that the keys can be
 * used in other classes.
 */

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);

signals:
    void KeyPressed(Qt::Key Key);
    void KeyReleased(Qt::Key Key);

private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // SCENE_H
