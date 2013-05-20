#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <vector>
#include <types.h>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * e);
    long s;
    std::vector<PacketInfo> packets;
    std::vector<long> currentlist;

};

#endif // SCENE_H
