#include "details.h"
#include "ui_details.h"
#include <iostream>

details::details(int nr, PacketInfo packet)
    /*:
    QWidget(parent),
    ui(new Ui::details)*/
{
    this->packet = packet;
    this->nr = nr;
    ui->setupUi(this);
    bg = new QGraphicsView(this);
    bg->setGeometry(20,200,600,400);
    sc = new QGraphicsScene();

    bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    bg->setScene(sc);
    bg->scale(1,1);
    sc->setSceneRect(0,0,600,500);
    tb = new QTextBrowser(this);
    tb->setGeometry(20,20,550,150);






}

void details::showinfo()
{
    pp = p.getTSPacket(packet);
    int l = 0, c=0;
    for(int i = 0; i<packet.length; i++)
    {
        if(i%16 == 0)
        {
            l+=30;
            c = 0;
        }
        if(i%8 == 0)
            c+=30;
    io = new QGraphicsTextItem;
    io->setPos(c+(i%16)*30,l);
    st = QString::number(pp.rawData[i],16).toUpper();
    io->setPlainText(st);
    io->font().setPointSize(30);
    sc->addItem(io);
    }
    pst = p.getPIDList();




    StreamType *streamTypes = new StreamType();
    QString typ = streamTypes->getStreamType(pst[packet.pid].streamType);
    st = QString::number(packet.pid,16).toUpper();

    QString info = QString("Packet nr.: %1\nPID: %2\nProgram number: %3\nStream type: %4").arg(nr).arg(st).arg(pst[packet.pid].programNumber).arg(typ);
    tb->setText(info);

}

details::~details()
{
    delete ui;
}
