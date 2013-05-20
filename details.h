#ifndef DETAILS_H
#define DETAILS_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include "types.h"
#include <stdlib.h>
#include <QPushButton>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGridLayout>
#include <QGraphicsSceneMouseEvent>
//#include "scene.h"
//#include "mywidget.h"
#include <QCheckBox>
#include <QGraphicsTextItem>

namespace Ui {
class details;
}

class details : public QWidget
{
    Q_OBJECT
    
public:
    explicit details();
    ~details();
    void showinfo(PacketInfo packet);
    
private:
    Ui::details *ui;
    QGraphicsView *bg;
    QGraphicsScene *sc;
    QGraphicsTextItem * io;
    QString st;
    PacketInfo *packet;

};

#endif // DETAILS_H
