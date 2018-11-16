#ifndef CHARACTERINTERFACE_H
#define CHARACTERINTERFACE_H

#include <QObject>

class CharacterInterface : public QObject
{
    Q_OBJECT
public:
    explicit CharacterInterface(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CHARACTERINTERFACE_H