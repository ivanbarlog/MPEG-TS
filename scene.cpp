#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include "mywidget.h"
#include "iostream"
#include <vector>

Scene::Scene() : QGraphicsScene()
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{

    long packetnr;
    int line, column;
    QPointF pt = e->scenePos();
    line = (((s/80)*8+50 - pt.y()))/8 ;
    column = (pt.x())/8;
    if(line*80+column <= currentlist.size())
    {
        packetnr = currentlist[line*80+column];
        std::cout << "packet.nr =  " << std::hex << packetnr << std::endl << "pid: " <<std::hex << packets[packetnr].pid << std::endl;
    }
    else
        std::cout << "missclick" << std::endl;
}
