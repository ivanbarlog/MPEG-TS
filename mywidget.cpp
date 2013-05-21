#include "mywidget.h"
#include "ui_mywidget.h"

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QPointF>


MyWidget::MyWidget()
{



    qp.color().black();
    qb.color().light();
    m_button = new QPushButton("My Button", this);
    m_button->setGeometry(QRect(QPoint(500, 600),
    QSize(200, 50)));
    connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));
    m_button->setText("Open File");

    map_bg = new QGraphicsView(this);
    map_bg->setGeometry(20,20,1100,500);
    scene = new Scene();

    map_bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    map_bg->setScene(scene);
    map_bg->scale(1,1);
    cb = new QCheckBox("PSI/SI",this);
    cb->setGeometry(1150,20,100,20);
    cb->setChecked(true);
    cb->setText("PSI/SI");
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,25,12,12);
    leg->setBackgroundBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);

    cboxes.push_back(cb);
    cb = new QCheckBox("Audio",this);
    cb->setGeometry(1150,60,100,20);
    cb->setChecked(true);
    cb->setText("Audio");
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,65,12,12);
    leg->setBackgroundBrush(QBrush(Qt::green, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);
    cboxes.push_back(cb);
    cb = new QCheckBox("Video",this);
    cb->setGeometry(1150,100,100,20);
    cb->setChecked(true);
    cb->setText("Video");
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,105,12,12);
    leg->setBackgroundBrush(QBrush(Qt::blue, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);
    cboxes.push_back(cb);
    cb = new QCheckBox("DSM-CC",this);
    cb->setGeometry(1150,140,100,20);
    cb->setChecked(true);
    cb->setText("DSM-CC");
    cboxes.push_back(cb);
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,145,12,12);
    leg->setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);
    cb = new QCheckBox("Null Packet",this);
    cb->setGeometry(1150,180,100,20);
    cb->setChecked(true);
    cb->setText("Null Packet");
    cboxes.push_back(cb);
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,185,12,12);
    leg->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);
    cb = new QCheckBox("Other",this);
    cb->setGeometry(1150,220,100,20);
    cb->setChecked(true);
    cb->setText("Other");
    cboxes.push_back(cb);
    leg = new QGraphicsView(this);
    leg->setGeometry(1130,225,12,12);
    leg->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    legsc = new QGraphicsScene(this);
    leg->setScene(legsc);

    r_button = new QPushButton("Refresh", this);
    r_button->setGeometry(QRect(QPoint(1150, 400),
    QSize(100, 50)));
    connect(r_button, SIGNAL(released()), this, SLOT(handleButton2()));
    r_button->setText("Refresh");





}

void MyWidget::paint()
{

    currentlist.clear();
    int r = 0;
    int k = 100;
    int q = 0, nr = 0;
    int off = 26, voff = 40;
    QString st;
    for(int ff = 0; ff < 10; ff++)
    {

        io = new QGraphicsTextItem;
        io->setPos(60+103*ff ,20);
        st = QString("%1 - %2").arg(ff*10).arg(ff*10+9);
        io->setPlainText(st);
        scene->addItem(io);
    }




   // scene->addText("trololo",)
    for(int j = 0; j <= (s / 100); j++)
    {
        io = new QGraphicsTextItem;
        io->setPos(0,70+j*110);
        st = QString("%1\n -\n%2k\n").arg(j).arg(j+1);
        io->setPlainText(st);
        scene->addItem(io);
        if(j % 10 == 0) voff+=10;



        off =25;
        k = 100;
        if(j == (s/100))
            k = s % 100;
        for(int i = 0; i < k; i++)
        {



            switch(pst[packets[q].pid].streamType)
            {
                case 0x1:
            case 0x2:
            case 0x10:   r = 2; break;
            case 0x3:
            case 0x4:
                case 0xF:
                case 0x11:
            case 0x81: r = 1; break;
            case 0x8:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:  r = 3; break;

            default: r = 5; break;

            }
            if(packets[q].pid == 0x1FFF)
                r = 4;

            if(packets[q].pid == 0 || programInfo.contains(packets[q].pid))
                r = 0;




          //  q++;
            currentlist.push_back(q++);
            if(i % 10 == 0) off+=4;

            if(cboxes[r]->isChecked() == false)
            continue;
            //    r = 7;



            switch(r)
            {
                case 0: c = Qt::yellow; break;
                case 1: c = Qt::green; break;
                case 2: c = Qt::blue; break;
                case 3: c = Qt::red; break;
                case 4: c = Qt::black; break;
                case 5: c = Qt::lightGray; break;
                case 6: c = Qt::darkCyan; break;
                case 7: c = Qt::lightGray; break;
                case 8: c = Qt::darkGray; break;
                case 9: c = Qt::darkGreen; break;
                case 10: c = Qt::magenta; break;
                case 11: c = Qt::darkRed; break;
                case 12: c = Qt::darkYellow; break;


                default: c = Qt::black; break;
            }



            scene->addRect(off + i*10,voff + j*10,9,9,QPen(Qt::white),QBrush(c, Qt::SolidPattern));
        //scene->mousePressEvent();

        }
    }

}


void MyWidget::handleButton()
{

    path = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));

    packets = p.getPacketList(path);

    s = packets.size();
    scene->setSceneRect(0,0,1050,(s/100)*11+100);
    scene->s = s;

    scene->packets = packets;
    pst = p.getPIDList();
    scene->pst = pst;
    scene->p = p;
    programInfo = p.getProgramInfo();
    paint();
    scene->currentlist = currentlist;


    /* usage */
   /* QHash<uint16_t, Program> programInfo;
    programInfo = p.getProgramInfo();

    Program prgrm;
    prgrm = programInfo[0x0020];

    std::vector<PMTProgram> v = prgrm.pmt.programs;
    PMTProgram asdf = v.at(0);

    //globalna premenna
    StreamType *streamTypes = new StreamType();
    QString typ = streamTypes->getStreamType(asdf.streamType);

    std::string text = typ.toUtf8().constData();
    std::cout << "Typ: " << text << std::endl;
*/
}

void MyWidget::handleButton2()
{
    scene->clear();
    paint();
    scene->currentlist = currentlist;
}

MyWidget::~MyWidget()
{
    delete ui;
}


