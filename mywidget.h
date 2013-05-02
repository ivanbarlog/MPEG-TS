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





private slots:
    void handleButton();




protected:
    void paint();
    QColor c;
    QPen qp;
    QBrush qb;



private:
    Ui::MyWidget *ui;
    QPushButton *m_button;
    Scene *scene;
    QGraphicsView *map_bg;
    QGridLayout *lo;

};

#endif // MYWIDGET_H
