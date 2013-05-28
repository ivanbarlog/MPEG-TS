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
#include <QComboBox>



namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget();
    ~MyWidget();
    int parts, part;
    QString path;
    long s;
    std::vector<PacketInfo> packets;
    std::vector<QCheckBox*> cboxes;
    std::vector<u_int64_t> currentlist,partstarts;
    QCheckBox *cb;
    QGraphicsTextItem * io;
    QHash<uint16_t, ProgramStreamType> pst;
    Parser p;
    QHash<uint16_t, Program> programInfo;
    QGraphicsView *leg;
    QGraphicsScene *legsc;
    int getpartscount(FILE *f);
    FILE *big;
    void setbuffer(int sp);





private slots:
    void handleButton();
    void handleButton2();
    void handleButton3();




protected:
    void paint();
    QColor c;
    QPen qp;
    QBrush qb;



private:
    Ui::MyWidget *ui;
    QPushButton *m_button, *r_button, *changepart;
    Scene *scene;
    QComboBox *combo;
    QGraphicsView *map_bg;
    QGridLayout *lo;
    int pid[20];

};

#endif // MYWIDGET_H
