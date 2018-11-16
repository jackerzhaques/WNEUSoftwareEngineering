#ifndef ABILITY_H
#define ABILITY_H

#include <QObject>

class Ability : public QObject
{
    Q_OBJECT
public:
    explicit Ability(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ABILITY_H