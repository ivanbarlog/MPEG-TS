#include "details.h"
#include "ui_details.h"
#include <iostream>

details::details(int nr, PacketInfo packet)
    /*:
    QWidget(parent),
    ui(new Ui::details)*/
{
    this->packet = packet;
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


    QHash<uint16_t, Program> programInfo;
    programInfo = p.getProgramInfo();

    Program prgrm;
/*
    if (programInfo.contains(packet.pid))
    {
        std::cerr << "Hello" << std::endl;
    }
    else
    {
        return;
    }
*/
    prgrm = programInfo[packet.pid];
/*
    std::vector<PMTProgram> v = prgrm.pmt.programs;

    for (std::vector<PMTProgram>::iterator itr = v.begin(); itr != v.end(); ++itr)
    {
        PMTProgram tmp = *itr;
        tmp.streamType
        tmp.elementaryPID
        //std::cout << "Program number: " << std::hex << tmp.programNumber << "; Program PID: " << std::hex<< tmp.programPID << std::endl;
    }
*/
    //PMTProgram asdf = v.at(0);
/*
    //globalna premenna
    StreamType *streamTypes = new StreamType();
    QString typ = streamTypes->getStreamType(asdf.streamType);
*/

    QString info = QString("Packet nr.: %1\nPID: %2\n").arg(nr).arg(packet.pid);
    tb->setText(info);



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


}

details::~details()
{
    delete ui;
}
