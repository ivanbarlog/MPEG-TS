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
    QGraphicsScene *scene;
    QGraphicsView *map_bg;
    std::vector<PacketInfo> packets;
    std::vector<QGraphicsRectItem*> rec;
};

#endif // MYWIDGET_H
