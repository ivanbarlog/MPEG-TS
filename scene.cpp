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
    int line, column, colmod, lmod;
    QPointF pt = e->scenePos();
    line = (pt.y() - 50) /110;
    lmod = ((int)pt.y() - 50) %110;
    if(lmod >=100) return;
    lmod/=10;
    line = line*10+lmod;
    column = (pt.x()-30)/104;
    colmod = ((int)pt.x()-30)%104;
    if(colmod >=100) return;
    colmod/=10;
    column = column*10+colmod;

    if(line*100+column < currentlist.size())
    {
        packetnr = currentlist[line*100+column];
        std::cout << "packet.nr =  " << std::hex << packetnr << std::endl << "pid: " <<std::hex << packets[packetnr].pid << std::endl;
        det = new details();

        det->show();
        det->showinfo(packets[packetnr]);

    }
    else
        std::cout << "missclick" << std::endl;
}
