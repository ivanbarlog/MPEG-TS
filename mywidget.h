#ifndef MYWIDGET_H
#define MYWIDGET_H

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
#include "scene.h"
#include <QCheckBox>
#include <QGraphicsTextItem>



namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget();
    ~MyWidget();
    int fff[1500];
    QString path;
    long s;
    std::vector<PacketInfo> packets;
    std::vector<QCheckBox*> cboxes;
    std::vector<long> currentlist;
    QCheckBox *cb;
    QGraphicsTextItem * io;






private slots:
    void handleButton();
    void handleButton2();




protected:
    void paint();
    QColor c;
    QPen qp;
    QBrush qb;



private:
    Ui::MyWidget *ui;
    QPushButton *m_button, *r_button;
    Scene *scene;
    QGraphicsView *map_bg;
    QGridLayout *lo;
    int pid[20];

};

#endif // MYWIDGET_H
