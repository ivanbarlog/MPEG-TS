#ifndef DETAILS_H
#define DETAILS_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include "types.h"
#include <stdlib.h>
#include <QPushButton>
#include <QFileDialog>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGridLayout>
#include <QGraphicsSceneMouseEvent>
//#include "scene.h"
//#include "mywidget.h"
#include <QCheckBox>
#include <QGraphicsTextItem>
#include "parser.h"

namespace Ui {
class details;
}

class details : public QWidget
{
    Q_OBJECT
    
public:
    explicit details(int nr,PacketInfo packet);
    ~details();
    void showinfo();
    Parser p;
    TSPacket pp;
    
private:
    Ui::details *ui;
    QGraphicsView *bg;
    QGraphicsScene *sc;
    QGraphicsTextItem * io;
    QString st;
    PacketInfo packet;
    QTextBrowser *tb;




};

#endif // DETAILS_H
