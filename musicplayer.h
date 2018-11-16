#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MUSICPLAYER_H