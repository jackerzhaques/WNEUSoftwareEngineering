#include "scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{

}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()){
        Qt::Key PressedKey = static_cast<Qt::Key>(event->key());
        emit(KeyPressed(PressedKey));
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()){
        Qt::Key ReleasedKey = static_cast<Qt::Key>(event->key());
        emit(KeyReleased(ReleasedKey));
    }
}
