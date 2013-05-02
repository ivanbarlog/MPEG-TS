#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include "mywidget.h"
#include "iostream"

Scene::Scene() : QGraphicsScene()
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{

    long packetnr;
    int line, column;
    QPointF pt = e->scenePos();
    line = (((s/80)*6+100 - pt.y())-50)/6 ;
    column = (pt.x()-50)/6;
    packetnr = line*80+column;
    std::cout << "packet.nr =  " << std::hex << packetnr << std::endl << "pid: " <<std::hex << packets[packetnr].pid << std::endl;
}
