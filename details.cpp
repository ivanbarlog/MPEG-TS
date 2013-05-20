#include "details.h"
#include "ui_details.h"

details::details()
    /*:
    QWidget(parent),
    ui(new Ui::details)*/
{
    ui->setupUi(this);
    bg = new QGraphicsView(this);
    bg->setGeometry(20,20,600,500);
    sc = new QGraphicsScene();

    bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    bg->setScene(sc);
    bg->scale(1,1);
    sc->setSceneRect(0,0,600,800);


}

void details::showinfo(PacketInfo packet)
{
    io = new QGraphicsTextItem;
    io->setPos(60,20);
    st = QString("%1  %2   %3").arg(packet.pid).arg(packet.length).arg(packet.start);
    io->setPlainText(st);
    sc->addItem(io);

}

details::~details()
{
    delete ui;
}
