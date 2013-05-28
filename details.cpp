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
     tw = new QTreeWidget(this);
   // tb = new QTextBrowser(this);
  //  tb->setGeometry(20,20,550,150);





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
    io->setToolTip(QString::number(pp.rawData[i],2));
    io->font().setPointSize(30);
    sc->addItem(io);
    }
    pst = p.getPIDList();




    StreamType *streamTypes = new StreamType();
    QString typ = streamTypes->getStreamType(pst[packet.pid].streamType);
    st = QString::number(packet.pid,16).toUpper();

    tw->setGeometry(20,20,550,150);
    tw->setHeaderLabel("Packet");

    //ts header
    twi = new QTreeWidgetItem();
    twi->setText(0,"MPEG-TS Header");
    QTreeWidgetItem *ch = new QTreeWidgetItem();
    ch->setText(0,"Sync Byte: 0x47");
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Transport Error Indicator: %1 ").arg(pp.tsheader.transportErrorIndicator));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Payload Unit Start Indicator: %1 ").arg(pp.tsheader.payloadUnitStartIndicator));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Transport Priority: %1 ").arg(pp.tsheader.transportPriority));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("PID: 0x%1 ").arg(QString::number(pp.tsheader.pid,16)));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Scrambling Control: %1 ").arg(pp.tsheader.scramblingControl));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Adaptation Field Control: %1 ").arg(pp.tsheader.adaptationFieldControl));
    twi->addChild(ch);
    ch = new QTreeWidgetItem();
    ch->setText(0,QString("Continuity Counter: %1 ").arg(pp.tsheader.continuityCounter));
    twi->addChild(ch);
    tw->addTopLevelItem(twi);

    //adaptation field
    if(pp.tsheader.adaptationFieldControl > 1)
    {
        twi = new QTreeWidgetItem();
        twi->setText(0,"Adaptation Field");
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Adaptation Field Lenght: %1 ").arg(pp.afheader.length));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Discontinuity Indicator: %1 ").arg(pp.afheader.discontinuityIndicator));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Random Access Indicator: %1 ").arg(pp.afheader.randomAccessIndicator));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Elementary stream priority indicator: %1 ").arg(pp.afheader.elementaryStreamPriorityIndicator));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("PCR Flag: %1 ").arg(pp.afheader.pcrFlag));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("OPCR Flag: %1 ").arg(pp.afheader.opcrFlag));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Splicing Point Flag: %1 ").arg(pp.afheader.splicingPointFlag));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Transport Private Data Flag: %1 ").arg(pp.afheader.transportPrivateDataFlag));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,QString("Adaptation Field Extension Flag: %1 ").arg(pp.afheader.extensionFlag));
        twi->addChild(ch);
        ch = new QTreeWidgetItem();
        ch->setText(0,"Adaptation Field Extension");
        twi->addChild(ch);
     //   QTreeWidgetItem *afex = new QTreeWidgetItem();

        tw->addTopLevelItem(twi);
    }

}

details::~details()
{
    delete ui;
}
