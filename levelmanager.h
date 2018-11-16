#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QObject>

class LevelManager : public QObject
{
    Q_OBJECT
public:
    explicit LevelManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LEVELMANAGER_H